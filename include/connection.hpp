#ifndef CONNECTION
#define CONNECTION
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/strand.hpp>
#include <handling.hpp>
#include <memory>
namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
using tcp = boost::asio::ip::tcp;
//? This is the interface that will deal with the sessions of our rest socket program
class session
{
protected:
    std::shared_ptr<handling> m_handling;
    beast::tcp_stream m_stream;
    beast::flat_buffer m_buffer;
    std::shared_ptr<std::string const> m_doc_root;
    http::request<http::string_body> m_req;

public:
    //? Make it the default behavior but still overridable
    virtual ~session() = default;
    //? Constructor
    session(
        tcp::socket &&socket,
        std::shared_ptr<std::string const> const &doc_root,
        std::shared_ptr<handling> handling)
        : m_stream(std::move(socket)), m_doc_root(doc_root), m_handling(handling)
    {
    }
    //? Run function
    virtual void run() = 0;
    //? Function to read requests
    virtual void do_read() = 0;
    //? What can we do on read
    virtual void on_read(
        beast::error_code ec,
        std::size_t bytes_transfered) = 0;
    //? Response so make a async write
    virtual void send_response(http::message_generator &&msg) = 0;
    //? What do to on writing
    virtual void on_write(
        bool keep_alive,
        beast::error_code ec,
        std::size_t bytes_transfered) = 0;
    //? To close the socket in case something goes wrong
    virtual void do_close() = 0;

private:
    //? GET PRIMARY PATH
    virtual std::string
    getPrimaryPath(std::string path) = 0;
    //? GET SEC PATH
    virtual std::string getSecPath(std::string left, std::string path) = 0;
};

//? This is the interface that will deal with listening to incoming requests
class listener
{
protected:
    net::io_context &m_ioc;
    tcp::acceptor m_acceptor;
    std::shared_ptr<std::string const> m_doc_root;
    tcp::endpoint m_endpoint;
    std::shared_ptr<handling> m_handling;

public:
    //? Make it default but still overridable
    virtual ~listener() = default;
    //? Open up the listener
    virtual void open_listener(
        beast::error_code ec) = 0;
    //? Allow reuse of the same ip and port
    virtual void allow_reuse_address(beast::error_code ec) = 0;
    //? Bind server address
    virtual void bind_server_address(beast::error_code ec) = 0;
    //? Start listening itself
    virtual void start_the_listening(beast::error_code ec) = 0;
    //? Constructor
    listener(
        net::io_context &ioc,
        tcp::endpoint endpoint,
        std::shared_ptr<std::string const> const &doc_root,
        std::shared_ptr<handling> handling)
        : m_ioc(ioc), m_acceptor(net::make_strand(ioc)), m_doc_root(doc_root), m_endpoint(endpoint), m_handling(handling)
    {
    }
    //? Actually start everything
    virtual void start_listener() = 0;
    //? To actually run the program
    virtual void run() = 0;

private:
    //? Accept requests
    virtual void do_accept() = 0;
    //? What to do on accept in case there are errors or not
    virtual void on_accept(beast::error_code ec, tcp::socket socket) = 0;
};
#endif