#ifndef HANDLING
#define HANDLING
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <map>
#include <memory>
#include <iostream>

namespace beast = boost::beast;
namespace http = beast::http;
enum class RegisterRoutesErrors
{
  OK = 0,
  DUPLICATED = 1,
  UNKNOWN_ERROR = 2,
  MAIN_ROUTE_DOES_NOT_EXIST = 3,
  ROUTE_WITH_METHOD_DONT_MATCH = 4,
  SUB_ROUTE_DOES_NOT_EXIST = 5,
  INITIALIZATION_ERROR = 6
};

struct RouteInfo
{
  //? SUBPATH
  std::string path;
  //? The actual behavior
  std::function<http::message_generator(http::request<http::string_body>)>
      handler;
  //? IF IT IS GET,POST,ETC..
  http::verb method;
  RouteInfo()
      : path(""),
        handler([](http::request<http::string_body>)
                { return http::response<http::string_body>{}; }),
        method(http::verb::get) {}
  RouteInfo(
      std::string &p,
      std::function<http::message_generator(http::request<http::string_body>)>
          &h,
      http::verb &m)
      : path(std::move(p)), handler(std::move(h)), method(std::move(m)) {}
  RouteInfo(
      std::string &&p,
      std::function<http::message_generator(http::request<http::string_body>)>
          &&h,
      http::verb &&m)
      : path(p), handler(h), method(m) {}
  RouteInfo(
      std::string &&p,
      std::function<http::message_generator(http::request<http::string_body>)>
          &h,
      http::verb &&m)
      : path(p), handler(std::move(h)), method(m) {}
};

class handling
{
protected:
  uint8_t m_size_main_routes;
  std::shared_ptr<std::string *> m_routes;
  std::shared_ptr<std::map<std::string, RouteInfo *>> m_sub_routes;
  std::shared_ptr<std::map<std::string, uint8_t>> m_sub_routes_size;
  bool initialized = false;

public:
  void init(
      uint8_t &size_main_routes)
  {
    //? INIT ROUTES
    m_size_main_routes = size_main_routes;
    std::string *routes = new std::string[size_main_routes];
    std::shared_ptr<std::string *> routes_wrapped =
        std::make_shared<std::string *>(routes);
    //? INIT SUB ROUTES MAPPING
    std::map<std::string, RouteInfo *> sub_routes_map;
    std::shared_ptr<std::map<std::string, RouteInfo *>> sub_routes_map_wrapped =
        std::make_shared<std::map<std::string, RouteInfo *>>(sub_routes_map);
    //? INIT SUB ROUTES SIZE MAP
    std::map<std::string, uint8_t> sub_routes_size_map;
    std::shared_ptr<std::map<std::string, uint8_t>>
        sub_routes_size_map_wrapped =
            std::make_shared<std::map<std::string, uint8_t>>(
                sub_routes_size_map);
    //? ASSIGN THE VALUES
    m_size_main_routes = size_main_routes;
    m_routes = std::move(routes_wrapped);
    m_sub_routes = std::move(sub_routes_map_wrapped);
    m_sub_routes_size = std::move(sub_routes_size_map_wrapped);
    initialized = true;
  }
  const uint8_t &get_routes_size() const { return m_size_main_routes; }
  const std::string *get_routes() const
  {
    auto h = m_routes.get();
    return *h;
  }
  const std::map<std::string, RouteInfo *> &get_sub_routes() const
  {
    auto h = m_sub_routes.get();
    return *h;
  }
  const std::map<std::string, uint8_t> &get_sub_routes_size() const
  {
    auto h = m_sub_routes_size.get();
    return *h;
  }
  handling() {}
  ~handling()
  {
    if (initialized)
    {
      //? WE ONLY NEED TO DELETE THOSE THAT WE INITIALIZED WITH THE KEYWORD NEW
      std::string *routes = *m_routes;
      std::map<std::string, RouteInfo *> &sub_routes_map = *m_sub_routes;
      //? GET THE MAIN ROUTES
      for (int i = 0; i < m_size_main_routes; i++)
      {
        delete[] sub_routes_map[routes[i]];
      }
      delete[] routes;
      m_routes.reset();
      m_sub_routes.reset();
      m_sub_routes_size.reset();
      m_size_main_routes = 0;
      initialized = false;
    }
  }
  //? Register main route
  virtual void register_main_route(std::string main_route,
                                   RegisterRoutesErrors &ec) = 0;
  //? Register sub route
  virtual void register_sub_route(std::string main_route,
                                  RouteInfo &sub_route,
                                  RegisterRoutesErrors &ec) = 0;
  //? Making the actual handling of calls according to the type of request and
  // the path for the request
  virtual std::function<
      http::message_generator(http::request<http::string_body>)>
  handle_by_mapping(std::string main_route, std::string sub_route,
                    http::verb method, RegisterRoutesErrors &err) = 0;
};
#endif