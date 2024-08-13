#include <functional>
#include <iostream>
#include <connection.hpp>
#include <error.hpp>
#include <debug.hpp>
namespace beast = boost::beast;
namespace http = beast::http;
namespace websocket = beast::websocket;
namespace net = boost::asio;
using tcp = boost::asio::ip::tcp;

class sessionImpl : public session, public std::enable_shared_from_this<sessionImpl>
{
public:
    std::string register_err_string_value(RegisterRoutesErrors error)
    {
        switch (error)
        {
        case RegisterRoutesErrors::OK:
            return "OK";
        case RegisterRoutesErrors::DUPLICATED:
            return "DUPLICATED";
        case RegisterRoutesErrors::UNKNOWN_ERROR:
            return "UNKNOWN_ERROR";
        case RegisterRoutesErrors::MAIN_ROUTE_DOES_NOT_EXIST:
            return "MAIN_ROUTE_DOES_NOT_EXIST";
        case RegisterRoutesErrors::ROUTE_WITH_METHOD_DONT_MATCH:
            return "ROUTE_WITH_METHOD_DONT_MATCH";
        case RegisterRoutesErrors::SUB_ROUTE_DOES_NOT_EXIST:
            return "SUB_ROUTE_DOES_NOT_EXIST";
        case RegisterRoutesErrors::INITIALIZATION_ERROR:
            return "INITIALIZATION_ERROR";
        default:
            return "UNKNOWN_ERROR";
        }
    }
    //? Create a session object
    sessionImpl(
        tcp::socket &&socket,
        std::shared_ptr<std::string const> const &doc_root,
        std::shared_ptr<handling> handling) : session(std::move(socket), doc_root, handling)
    {
    }
    //? Start a asynchronous operation
    void
    run() override
    {
        // We need to be executing within a strand to perform async operations
        // on the I/O objects in this session. Although not strictly necessary
        // for single-threaded contexts, this example code is written to be
        // thread-safe by default.
        net::dispatch(m_stream.get_executor(),
                      beast::bind_front_handler(
                          &sessionImpl::do_read,
                          shared_from_this()));
    }

    //? Assync read
    void do_read() override
    {
        //?Creating a empty object
        m_req = {};
        //?Set timeout
        m_stream.expires_after(std::chrono::seconds(30));
        //? Read a request
        http::async_read(m_stream, m_buffer, m_req, beast::bind_front_handler(&sessionImpl::on_read, shared_from_this()));
    }

    //? ON read what can we do
    void on_read(
        beast::error_code ec,
        std::size_t bytes_transfered) override
    {
        //? IGNORE EMPTY BYTES
        boost::ignore_unused(bytes_transfered);
        //? This means that the connection got closed
        if (ec == http::error::end_of_stream)
        {
            return do_close();
        }
        //? Handle normal error
        if (ec)
        {
            LOG_W("Error in on_read() sessionimpl");
            return fail(ec, "read");
        }

        //? SEND RESPONSE
        send_response(
            handle_request(*m_doc_root, std::move(m_req)));
    }
    //? To send a response we make a async write
    void send_response(http::message_generator &&msg) override
    {
        bool keep_alive = msg.keep_alive();
        beast::async_write(
            m_stream,
            std::move(msg),
            beast::bind_front_handler(
                &sessionImpl::on_write, shared_from_this(), keep_alive));
    }

    void on_write(
        bool keep_alive,
        beast::error_code ec,
        std::size_t bytes_transfered) override
    {
        boost::ignore_unused(bytes_transfered);
        if (ec)
        {
            LOG_W("Error in on_write() in sessionImpl");
            return fail(ec, "write");
        }
        //? close in case its not alive
        if (!keep_alive)
        {
            return do_close();
        }
        //? READ ANOTHER REQUEST
        do_read();
    }

    void do_close() override
    {
        beast::error_code ec;
        m_stream.socket().shutdown(tcp::socket::shutdown_send, ec);
    }

private:
    template <class Body, class Allocator>
    http::message_generator handle_request(beast::string_view doc_root, http::request<Body, http::basic_fields<Allocator>> &&req)
    {
        http::response<http::string_body> res{http::status::accepted, req.version()};
        std::string path = req.target();
        //? IGNORING THE /favicon.ico request
        if (path == "/favicon.ico")
        {
            LOG_W("Returning the not found to favicon");
            res.result(http::status::not_found);
            res.set(http::field::content_type, "text/plain");
            res.body() = "404 Not Found";
            res.prepare_payload();
            return res;
        }

        std::string left = getPrimaryPath(req.target());
        std::string right = getSecPath(left, path);
        LOG_W("*******************************************");
        LOG_W("Request made to the following main path:");
        LOG_W(left);
        LOG_W("Where the sub path was:");
        LOG_W(right);
        LOG_W("*******************************************");
        RegisterRoutesErrors err = RegisterRoutesErrors::OK;
        std::function<http::message_generator(http::request<http::string_body>)> func = (*m_handling).handle_by_mapping(left, right, req.method(), err);
        if (err != RegisterRoutesErrors::OK)
        {
            LOG_W("Unable to find the correct function");
            res.result(http::status::not_found);
            res.set(http::field::content_type, "text/plain");
            res.body() = "404 Not Found with code " + register_err_string_value(err);
            res.prepare_payload();
            return res;
        }
        return func(req);
    }

    std::string getPrimaryPath(std::string path) override
    {
        std::string copy = "/";
        int left = path.find_first_of("/");
        for (int i = left + 1; i < path.length(); i++)
        {
            copy.append(std::string{path[i]});
            if (path[i] == '/')
            {
                break;
            }
        }
        if (copy[copy.length() - 1] != '/')
        {
            copy.append(std::string{'/'});
        }
        return copy;
    }
    std::string getSecPath(std::string left, std::string path) override
    {
        std::string result = "";
        if (path[path.length() - 1] != '/')
        {
            path.append(std::string{'/'});
        }
        if (left == path)
        {
            result.append(std::string{'/'});
            return result;
        }
        for (int i = left.length() - 1; i < path.length(); i++)
        {
            if (path[i] == '?')
            {
                break;
            }
            result.append(std::string{path[i]});
        }
        if (result[result.length() - 1] != '/')
        {
            result.append(std::string{'/'});
        }
        return result;
    }
};

class listenerImpl : public listener, public std::enable_shared_from_this<listenerImpl>
{
public:
    listenerImpl(
        net::io_context &ioc,
        tcp::endpoint endpoint,
        std::shared_ptr<std::string const> const &doc_root,
        std::shared_ptr<handling> handling) : listener(ioc, endpoint, doc_root, handling)
    {
        start_listener();
    }
    //? CREATE THE LISTENING
    void start_listener() override
    {
        beast::error_code ec;
        open_listener(ec);
        allow_reuse_address(ec);
        bind_server_address(ec);
        start_the_listening(ec);
    }

    //? Open up the listener
    virtual void open_listener(
        beast::error_code ec) override
    {
        //? open the listener
        m_acceptor.open(m_endpoint.protocol(), ec);
        if (ec)
        {
            LOG_W("Error in opening the listener (function open_listener listenerImpl)");
            fail(ec, "open");
            return;
        }
    }
    //? Allow reuse of the same ip and port
    virtual void allow_reuse_address(beast::error_code ec) override
    {
        //? allow address reuse (having more than 1 linkage to the same ip and address)
        m_acceptor.set_option(net::socket_base::reuse_address(true), ec);
        if (ec)
        {
            LOG_W("Ip reuse error");
            fail(ec, "set_option");
            return;
        }
    }
    //? Bind server address
    virtual void bind_server_address(beast::error_code ec) override
    {
        //? bind the server address
        m_acceptor.bind(m_endpoint, ec);
        if (ec)
        {
            LOG_W("Error in the binding of the ip");
            fail(ec, "bind");
            return;
        }
    }
    //? Start listening itself
    virtual void start_the_listening(beast::error_code ec) override
    {
        //? start listening
        m_acceptor.listen(net::socket_base::max_listen_connections, ec);
        if (ec)
        {
            LOG_W("Error start listening");
            fail(ec, "listen");
            return;
        }
    }

    //? FOR START
    void run() override
    {
        do_accept();
    }

private:
    void do_accept() override
    {
        m_acceptor.async_accept(
            net::make_strand(m_ioc),
            beast::bind_front_handler(
                &listenerImpl::on_accept,
                shared_from_this()));
    }
    void on_accept(beast::error_code ec, tcp::socket socket) override
    {
        if (ec)
        {
            LOG_W("Error on accept connection");
            fail(ec, "accept");
            return;
        }
        else
        {
            std::make_shared<sessionImpl>(
                std::move(socket),
                m_doc_root,
                m_handling)
                ->run();
        }
        do_accept();
    }
};