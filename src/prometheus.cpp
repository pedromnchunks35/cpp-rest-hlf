#ifndef PROMETHEUS_IMPL
#define PROMETHEUS_IMPL
#include <prometheus.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <debug.hpp>
namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
using tcp = net::ip::tcp;
class prometheusImpl : public prometheus
{
public:
    prometheusImpl(const std::string &hostname, const std::string &port, const std::string &endpoint)
        : prometheus(hostname, port, endpoint) {};
    //? Make the actual connection that returns a response
    http::response<http::string_body> make_connection(http::request<http::string_body> req, std::string &query) override
    {
        try
        {
            std::string query_copy = query;
            //? GET VERSION
            int const version_int = m_version == "1.0" ? 10 : 11;
            //? INITING THE IO RESOURCES
            net::io_context ioc;
            tcp::resolver resolver(ioc);
            beast::tcp_stream stream(ioc);
            //? MAKE A LOOKUP
            auto const lookup_result = resolver.resolve(m_hostname, m_port);
            //? CONFIG CONNECTION CONFIGS
            stream.connect(lookup_result);
            //? CREATE REQUEST
            std::string params = getQueryParams(req.target());
            std::string copy_m_endpoint = m_endpoint;
            std::string url_request = copy_m_endpoint.append("?query=").append(query_copy).append("&").append(params);
            http::request<http::string_body> req{http::verb::get, url_request, version_int};
            req.set(http::field::host, m_hostname);
            req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
            LOG_W("***************************************");
            LOG_W("Making a request with the following host:");
            LOG_W(m_hostname);
            LOG_W("Following endpoint:");
            LOG_W(m_endpoint);
            LOG_W("Following Port:");
            LOG_W(m_port);
            LOG_W("Following query:");
            LOG_W(query_copy);
            LOG_W("Following url_request:");
            LOG_W(url_request);
            LOG_W("***************************************");
            //? SEND THE REQUEST
            http::write(stream, req);
            //? READ RESPONSE
            beast::flat_buffer buffer;
            http::response<http::string_body> res;
            http::read(stream, buffer, res);
            //? CLOSE RESOURCES
            beast::error_code ec;
            stream.socket().shutdown(tcp::socket::shutdown_both, ec);
            if (ec && ec != beast::errc::not_connected)
            {
                LOG_W("Error in shuting down the connection prometheus");
                return res;
            }
            LOG_W(res);
            return res;
        }
        catch (const std::exception &e)
        {
            http::response<http::string_body> res;
            res.body() = "{\"message\":\"Something went wrong with the connection to the prometheus instance\"}";
            res.prepare_payload();
            LOG_W("Some error occurred trying to communicate with prometheus");
            LOG_W(e.what());
            return res;
        }
    }
    //? Just make the normal query to the postman
    boost::json::object make_query(http::request<http::string_body> req, std::string &query) override
    {
        http::response<http::string_body> res = make_connection(req, query);
        boost::json::value v = boost::json::parse(res.body());
        if (v.is_object())
        {
            return v.as_object();
        }
        else
        {
            boost::json::object j;
            return j;
        }
    }
    std::string getQueryParams(std::string path)
    {
        int k = path.find_first_of('?');
        std::string query = "";
        for (int i = k + 1; i < path.length(); i++)
        {
            query += path[i];
        }
        return query;
    }
};
#endif