#ifndef PROMETHEUS
#define PROMETHEUS
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/json.hpp>
namespace beast = boost::beast;
namespace http = beast::http;
class prometheus
{
protected:
  std::string const m_hostname;
  std::string const m_port;
  std::string m_endpoint;
  std::string const m_version;

public:
  prometheus() {}
  prometheus(const std::string &hostname, const std::string &port,
             const std::string &endpoint)
      : m_hostname(std::move(hostname)), m_port(std::move(port)), m_endpoint(std::move(endpoint)), m_version(std::move("1.0")) {}
  //? Make the actual connection that returns a response
  virtual http::response<http::string_body> make_connection(http::request<http::string_body> req, std::string &query) = 0;
  //? Just make the normal query to the postman
  virtual boost::json::object make_query(http::request<http::string_body> req, std::string &query) = 0;
};
#endif