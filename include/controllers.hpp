#ifndef CONTROLLERS
#define CONTROLLERS
#include <iostream>
#include <functional>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <memory>
#include "prometheus.hpp"
namespace beast = boost::beast;
namespace http = beast::http;
using rest_response = std::function<http::message_generator(http::request<http::string_body>)>;
class controllers
{
protected:
    std::shared_ptr<prometheus> m_prometheus;

public:
    explicit controllers(
        std::shared_ptr<prometheus> prometheus)
        : m_prometheus(std::move(prometheus)) {}
    /***********************************************************************************/
    virtual rest_response main_route() = 0;
    /***********************************************************************************/
    virtual rest_response cpu_variation_perc_controller() = 0;
    virtual rest_response cpu_variation_per_machine_controller() = 0;
    /***********************************************************************************/
    virtual rest_response ram_gb_per_component_controller() = 0;
    virtual rest_response ram_gb_per_machine_controller() = 0;
    virtual rest_response ram_gb_per_component_perc_controller() = 0;
    virtual rest_response ram_gb_per_machine_perc_controller() = 0;
    /***********************************************************************************/
    virtual rest_response disk_usage_per_component_controller() = 0;
    virtual rest_response disk_usage_per_machine_controller() = 0;
    virtual rest_response disk_usage_per_component_perc_controller() = 0;
    virtual rest_response disk_usage_per_machine_perc_controller() = 0;
    /***********************************************************************************/
    virtual rest_response ledger_per_machine_gb_controller() = 0;
    virtual rest_response ledger_per_machine_gb_perc_controller() = 0;
    /***********************************************************************************/
    virtual rest_response io_number_ops_per_component_controller() = 0;
    virtual rest_response io_number_ops_per_machine_controller() = 0;
    virtual rest_response io_var_number_of_io_per_minute_per_component_controller() = 0;
    virtual rest_response io_var_number_of_io_per_machine_per_minute_controller() = 0;
    virtual rest_response io_number_of_reads_per_component_controller() = 0;
    virtual rest_response io_number_of_reads_per_machine_controller() = 0;
    virtual rest_response io_var_number_of_reads_per_minute_per_component_controller() = 0;
    virtual rest_response io_var_number_cumulative_reads_per_machine_per_minute_controller() = 0;
    virtual rest_response io_number_writes_per_machine_controller() = 0;
    virtual rest_response io_number_writes_per_component_controller() = 0;
    virtual rest_response io_var_number_writes_per_minute_per_component_controller() = 0;
    virtual rest_response io_number_writes_per_machine_per_minute_controller() = 0;
    /***********************************************************************************/
    virtual rest_response net_number_received_per_component_controller() = 0;
    virtual rest_response net_number_io_received_per_machine_controller() = 0;
    virtual rest_response net_var_received_per_minute_per_component_controller() = 0;
    virtual rest_response net_number_io_received_per_machine_per_minute_controller() = 0;
    virtual rest_response net_number_transmited_per_component_controller() = 0;
    virtual rest_response net_transmited_per_machine_controller() = 0;
    virtual rest_response net_var_transmited_per_component_per_minute_controller() = 0;
    virtual rest_response net_transmited_per_machine_per_minute_controller() = 0;
    /***********************************************************************************/
};
#endif