#include <iostream>
#include <gtest/gtest.h>
#include "prometheus_mock.hpp"
#include <boost/json.hpp>
class Initializer : public testing::Test
{
public:
    Initializer() {}
    ~Initializer() {}
    void SetUp() {}
    void TearDown() {}
    prometheusMock prometheus_mock;
};

TEST_F(Initializer, prometheus_test)
{
    boost::json::object obj{
        {"status", "success"},
        {"data", boost::json::object{
                     {"resultType", "vector"},
                     {"result", boost::json::array{
                                    boost::json::object{
                                        {"metric", boost::json::object{
                                                       {"__name__", "http_requests_total"},
                                                       {"method", "get"},
                                                       {"status", "200"}}},
                                        {"value", boost::json::array{"1625214151", "1027"}}}}}}}};
    http::response<http::string_body> res;
    http::request<http::string_body> req;
    res.body() = boost::json::serialize(obj);
    res.prepare_payload();
    EXPECT_CALL(prometheus_mock, make_connection(testing::_, testing::_)).Times(1).WillOnce(testing::Return(res));
    std::string test = "";
    http::response<http::string_body> res_call = prometheus_mock.make_connection(req, test);
    boost::json::value v = boost::json::parse(res_call.body());
    boost::json::object obj2 = v.as_object();
    ASSERT_EQ(obj2, obj);
    std::string query = "http_requests_total{method=\" get \", status=\" 200 \"}";
    EXPECT_CALL(
        prometheus_mock,
        make_query(testing::_, testing::_))
        .Times(1)
        .WillOnce(testing::Return(obj));
    boost::json::object obj3 = prometheus_mock.make_query(req, query);
    ASSERT_EQ(obj3, obj);
}