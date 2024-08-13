#include <gmock/gmock.h>

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <iostream>
#include <map>
#include <memory>

#include "handling.hpp";
namespace beast = boost::beast;
namespace http = beast::http;

class handlingMock : public handling
{
public:
    handlingMock() : handling() {}
    // Mocking register_main_route
    MOCK_METHOD(void, register_main_route,
                (std::string main_route, RegisterRoutesErrors &ec), (override));

    // Mocking register_sub_route
    MOCK_METHOD(void, register_sub_route,
                (std::string main_route, RouteInfo &sub_route,
                 RegisterRoutesErrors &ec),
                (override));

    // Mocking handle_by_mapping
    MOCK_METHOD(
        std::function<http::message_generator(http::request<http::string_body>)>,
        handle_by_mapping,
        (std::string main_route, std::string sub_route, http::verb method, RegisterRoutesErrors &err),
        (override));
};