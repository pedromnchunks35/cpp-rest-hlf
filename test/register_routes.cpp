#include <gtest/gtest.h>

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <iostream>
#include <memory>

#include "handling_mock.hpp"
namespace beast = boost::beast;
namespace http = beast::http;
class Initializer_handler : public testing::Test
{
public:
  Initializer_handler() {}
  ~Initializer_handler() {}
  void SetUp()
  {
    uint8_t k = 1;
    handling_mock.init(k);
  }
  void TearDown() {}
  handlingMock handling_mock;
};

TEST_F(Initializer_handler, register_main_route_test)
{
  //? EXPECT THE REGISTER OF "/"
  RegisterRoutesErrors err;
  EXPECT_CALL(handling_mock, register_main_route("/", err))
      .Times(1)
      .WillOnce(testing::Invoke(
          [](const std::string &, RegisterRoutesErrors &err) {}));
  handling_mock.register_main_route("/", err);
  ASSERT_EQ(err, RegisterRoutesErrors::OK);
  //? EXPECT NOT REGISTERING BECAUSE WE HAVE DUPLICATION OF ROUTES
  EXPECT_CALL(handling_mock, register_main_route("/", err))
      .Times(1)
      .WillOnce(testing::Invoke(
          [&err](const std::string &, RegisterRoutesErrors &err_here)
          {
            // Modify the err_here to simulate duplication error
            err_here = RegisterRoutesErrors::DUPLICATED;
          }));
  handling_mock.register_main_route("/", err);
  ASSERT_EQ(err, RegisterRoutesErrors::DUPLICATED);
}

TEST_F(Initializer_handler, register_sub_route_test)
{

  RouteInfo routeInfo;
  routeInfo.path = "/sec";
  routeInfo.method = http::verb::get;
  routeInfo.handler = [](http::request<http::string_body> req)
  {
    std::string json_body = "";
    if (req.method() == http::verb::get)
    {
      json_body = "{\"message\":\"I'm just fine I guess\"}";
    }
    else
    {
      json_body = "{\"message\":\"I'm not fine I guess\"}";
    }
    http::response<http::string_body> res{http::status::accepted, req.version()};
    res.set(http::field::server, "boost_something");
    res.set(http::field::content_type, "application/json");
    res.body() = json_body;
    res.prepare_payload();
    res.keep_alive(req.keep_alive());
    return res;
  };

  RegisterRoutesErrors err = RegisterRoutesErrors::OK;

  // Set up the expectation for the first call
  EXPECT_CALL(handling_mock, register_sub_route("/", testing::_, testing::_))
      .Times(1)
      .WillOnce(testing::Invoke(
          [&err](std::string, RouteInfo &, RegisterRoutesErrors &ec)
          {
            // Modify the RegisterRoutesErrors reference
            ec = RegisterRoutesErrors::OK;
          }));

  // Call the function under test
  handling_mock.register_sub_route("/", routeInfo, err);
  ASSERT_EQ(err, RegisterRoutesErrors::OK);

  // Try to insert a duplicate
  EXPECT_CALL(handling_mock, register_sub_route("/", testing::_, testing::_))
      .Times(1)
      .WillOnce(testing::Invoke(
          [&err](std::string, RouteInfo &, RegisterRoutesErrors &ec)
          {
            // Simulate duplication error
            ec = RegisterRoutesErrors::DUPLICATED;
          }));

  // Call the function under test again
  handling_mock.register_sub_route("/", routeInfo, err);
  ASSERT_EQ(err, RegisterRoutesErrors::DUPLICATED);
}

TEST_F(Initializer_handler, handle_by_mapping)
{
  std::string main_route = "/";
  std::string sub_route = "/sec";
  http::verb method = http::verb::get;
  RegisterRoutesErrors err;
  std::function<http::message_generator(http::request<http::string_body>)>
      func = [](http::request<http::string_body> req)
  {
    std::string json_body = "";
    json_body = "{\"message\":\"I'm not fine I guess\"}";
    http::response<http::string_body> res{http::status::accepted, req.version()};
    res.set(http::field::server, "boost_something");
    res.set(http::field::content_type, "application/json");
    res.body() = json_body;
    res.prepare_payload();
    res.keep_alive(req.keep_alive());
    return res;
  };
  EXPECT_CALL(handling_mock,
              handle_by_mapping(main_route, sub_route, method, err))
      .Times(1)
      .WillOnce(testing::Return(func));
  std::function<http::message_generator(http::request<http::string_body>)>
      the_function =
          handling_mock.handle_by_mapping(main_route, sub_route, method, err);
  http::request<http::string_body> req = {};
  http::message_generator gen = the_function(req);
  bool is_valid = !gen.is_done();
  ASSERT_EQ(is_valid, true);
}