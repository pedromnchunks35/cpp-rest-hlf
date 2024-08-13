#ifndef PROMETHEUS_MOCK
#define PROMETHEUS_MOCK
#include <iostream>
#include "prometheus.hpp"
#include <gmock/gmock.h>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/json.hpp>
namespace beast = boost::beast;
namespace http = beast::http;
class prometheusMock : public prometheus
{
public:
    prometheusMock() {}
    prometheusMock(std::string hostname, std::string port, std::string endpoint) : prometheus(hostname, port, endpoint) {}
    MOCK_METHOD(http::response<http::string_body>, make_connection, (http::request<http::string_body> req,std::string & query), (override));
    MOCK_METHOD(boost::json::object, make_query, (http::request<http::string_body> req,std::string & query), (override));
};
#endif