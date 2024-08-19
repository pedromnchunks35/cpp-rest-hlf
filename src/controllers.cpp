#ifndef CONTROLLERS_IMPL
#define CONTROLLERS_IMPL
#include <controllers.hpp>
#include "prometheus.cpp"
class controllersImpl : public controllers
{
public:
    controllersImpl(
        std::shared_ptr<prometheus> prometheus)
        : controllers(prometheus)
    {
    }
    /***********************************************************************************/
    rest_response main_route() override
    {
        std::function<http::message_generator(http::request<http::string_body>)> handler = [](http::request<http::string_body> req) -> http::message_generator
        {
            //? CREATE THE ACTUAL RESPONSE
            http::response<http::string_body> res{http::status::accepted, req.version()};
            res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
            res.set(http::field::content_type, "application/json");
            res.body() = "{\"message\":\"Hello World\"}";
            res.set(http::field::access_control_allow_origin, "*");
            res.set(http::field::access_control_allow_methods, "GET, POST, PUT, DELETE, OPTIONS");
            res.set(http::field::access_control_allow_headers, "Content-Type");
            res.prepare_payload();
            res.keep_alive(req.keep_alive());
            return res;
        };
        return handler;
    }
    /***********************************************************************************/
    rest_response cpu_variation_perc_controller() override
    {
        std::string query = "sum(rate(process_cpu_seconds_total{job=\"operationsComb\"}[1m]))*100";
        return give_general_response(query);
    }
    rest_response cpu_variation_per_machine_controller() override
    {
        std::string query = "sum(rate(container_cpu_usage_seconds_total{job=\"machinesComb\",name!=\"cadvisor\",name!=\"explorer\",name!=\"explorerdb\"}[1m]))*100";
        return give_general_response(query);
    }
    /***********************************************************************************/
    rest_response ram_gb_per_component_controller() override
    {
        std::string query = "sum(container_memory_usage_bytes{job=\"machinesComb\",image!=\"\",name!=\"cadvisor\",name!=\"explorer\",name!=\"explorerdb\",name!~\".*-basic.*\",name!~\".*.couch.*\"}/(1024^3))";
        return give_general_response(query);
    }
    rest_response ram_gb_per_machine_controller() override
    {
        std::string query = "sum(container_memory_usage_bytes{job=\"machinesComb\",image!=\"\",name!=\"cadvisor\",name!=\"explorer\",name!=\"explorerdb\"}/(1024^3))";
        return give_general_response(query);
    }
    rest_response ram_gb_per_component_perc_controller() override
    {
        std::string query = "((sum(container_memory_usage_bytes{job=\"machinesComb\",image!=\"\",name!=\"cadvisor\",name!=\"explorer\",name!=\"explorerdb\",name!~\".*-basic.*\",name!~\".*-couch.*\"}/(1024^3)))/48)*100";
        return give_general_response(query);
    }
    rest_response ram_gb_per_machine_perc_controller() override
    {
        std::string query = "((sum(container_memory_usage_bytes{job=\"machinesComb\",image!=\"\",name!=\"cadvisor\",name!=\"explorer\",name!=\"explorerdb\"}/(1024^3)))/48)*100";
        return give_general_response(query);
    }
    /***********************************************************************************/
    rest_response disk_usage_per_component_controller() override
    {
        std::string query = "sum(container_fs_usage_bytes{job=\"machinesComb\",image!=\"\",name!=\"cadvisor\",name!=\"explorer\",name!=\"explorerdb\",name!~\".*-basic.*\",name!~\".*couch.*\"})/(1024^3)";
        return give_general_response(query);
    }
    rest_response disk_usage_per_machine_controller() override
    {
        std::string query = "sum(container_fs_usage_bytes{job=\"machinesComb\",image!=\"\",name!=\"cadvisor\",name!=\"explorer\",name!=\"explorerdb\",name!~\".*-basic.*\",name!~\".*couch.*\"})/(1024^3)";
        return give_general_response(query);
    }
    rest_response disk_usage_per_component_perc_controller() override
    {
        std::string query = "((sum(container_fs_usage_bytes{job=\"machinesComb\",image!=\"\",name!=\"cadvisor\",name!=\"explorer\",name!=\"explorerdb\",name!~\".*-basic.*\",name!~\".*couch.*\"})/(1024^3))/(((sum(topk(1,container_fs_limit_bytes{job=\"machinesComb\"})))/(1024^3))*2))*100";
        return give_general_response(query);
    }
    rest_response disk_usage_per_machine_perc_controller() override
    {
        std::string query = "((sum(container_fs_usage_bytes{job=\"machinesComb\",name!=\"cadvisor\",image!=\"\",name!=\"explorer\",name!=\"explorerdb\",name!~\".*-basic.*\",name!~\".*couch.*\"})/(1024^3))/(((sum(topk(1,container_fs_limit_bytes{job=\"machinesComb\"})))/(1024^3))*2))*100";
        return give_general_response(query);
    }
    /***********************************************************************************/
    rest_response ledger_per_machine_gb_controller() override
    {
        std::string query = "(sum(container_fs_usage_bytes{job=\"machinesComb\",image!=\"\",name=~\".*.couch.*\"}))/(1024^3)";
        return give_general_response(query);
    }
    rest_response ledger_per_machine_gb_perc_controller() override
    {
        std::string query = "(((sum(container_fs_usage_bytes{job=\"machinesComb\",image!=\"\",name=~\".*.couch.*\"}))/(1024^3))/(((sum(topk(1,container_fs_limit_bytes{job=\"machinesComb\"})))/(1024^3))*2))*100";
        return give_general_response(query);
    }
    /***********************************************************************************/
    rest_response io_number_ops_per_component_controller() override
    {
        std::string query = "sum(container_fs_io_current{job=\"machinesComb\",name!=\"cadvisor\",name!=\"explorer\",name!=\"explorerdb\"})";
        return give_general_response(query);
    }
    rest_response io_number_ops_per_machine_controller() override
    {
        std::string query = "sum(container_fs_io_current{job=\"machinesComb\",name!=\"cadvisor\",name!=\"explorer\",name!=\"explorerdb\"})";
        return give_general_response(query);
    }
    rest_response io_var_number_of_io_per_minute_per_component_controller() override
    {
        std::string query = "sum(rate(container_fs_io_current{job=\"machinesComb\",name!=\"cadvisor\",name!=\"explorer\",name!=\"explorerdb\"}[1m]))*60";
        return give_general_response(query);
    }
    rest_response io_var_number_of_io_per_machine_per_minute_controller() override
    {
        std::string query = "sum(rate(container_fs_io_current{job=\"machinesComb\",image!=\"\",name!=\"cadvisor\",name!=\"explorer\",name!=\"explorerdb\"}[1m]))*60";
        return give_general_response(query);
    }
    rest_response io_number_of_reads_per_component_controller() override
    {
        std::string query = "sum(container_fs_reads_total{job=\"machinesComb\",name!=\"cadvisor\",name!=\"explorer\",name!=\"explorerdb\"})";
        return give_general_response(query);
    }
    rest_response io_number_of_reads_per_machine_controller() override
    {
        std::string query = "sum(container_fs_reads_total{job=\"machinesComb\",name!=\"cadvisor\",name!=\"explorer\",name!=\"explorerdb\"})";
        return give_general_response(query);
    }
    rest_response io_var_number_of_reads_per_minute_per_component_controller() override
    {
        std::string query = "sum(rate(container_fs_reads_total{job=\"machinesComb\",name!=\"cadvisor\",name!=\"explorer\",name!=\"explorerdb\"}[1m]))*60";
        return give_general_response(query);
    }
    rest_response io_var_number_cumulative_reads_per_machine_per_minute_controller() override
    {
        std::string query = "sum(rate(container_fs_reads_total{job=\"machinesComb\",name!=\"cadvisor\",name!=\"explorer\",name!=\"explorerdb\"}[1m]))*60";
        return give_general_response(query);
    }
    rest_response io_number_writes_per_machine_controller() override
    {
        std::string query = "sum(container_fs_writes_total{job=\"machinesComb\",name!=\"cadvisor\",name!=\"explorer\",name!=\"explorerdb\"})";
        return give_general_response(query);
    }
    rest_response io_number_writes_per_component_controller() override
    {
        std::string query = "sum(container_fs_writes_total{job=\"machinesComb\",name!=\"cadvisor\",name!=\"explorer\",name!=\"explorerdb\"})";
        return give_general_response(query);
    }
    rest_response io_var_number_writes_per_minute_per_component_controller() override
    {
        std::string query = "sum(rate(container_fs_writes_total{job=\"machinesComb\",name!=\"cadvisor\",name!=\"explorer\",name!=\"explorerdb\"}[1m]))*60";
        return give_general_response(query);
    }
    rest_response io_number_writes_per_machine_per_minute_controller() override
    {
        std::string query = "sum(rate(container_fs_reads_total{job=\"machinesComb\",name!=\"cadvisor\",name!=\"explorer\",name!=\"explorerdb\"}[1m]))*60";
        return give_general_response(query);
    }
    /***********************************************************************************/
    rest_response net_number_received_per_component_controller() override
    {
        std::string query = "sum(container_network_receive_packets_total{job=\"machinesComb\",name!=\"cadvisor\",name!=\"explorer\",name!=\"explorerdb\"})";
        return give_general_response(query);
    }
    rest_response net_number_io_received_per_machine_controller() override
    {
        std::string query = "sum(container_network_receive_packets_total{job=\"machinesComb\",name!=\"cadvisor\",name!=\"explorer\",name!=\"explorerdb\"})";
        return give_general_response(query);
    }
    rest_response net_var_received_per_minute_per_component_controller() override
    {
        std::string query = "sum(rate(container_network_receive_packets_total{job=\"machinesComb\",name!=\"cadvisor\",name!=\"explorer\",name!=\"explorerdb\"}[1m]))*60";
        return give_general_response(query);
    }
    rest_response net_number_io_received_per_machine_per_minute_controller() override
    {
        std::string query = "sum(container_network_receive_packets_total{job=\"machinesComb\",name!=\"cadvisor\",name!=\"explorer\",name!=\"explorerdb\"})";
        return give_general_response(query);
    }
    rest_response net_number_transmited_per_component_controller() override
    {
        std::string query = "sum(container_network_transmit_packets_total{job=\"machinesComb\",name!=\"cadvisor\",name!=\"explorer\",name!=\"explorerdb\"})";
        return give_general_response(query);
    }
    rest_response net_transmited_per_machine_controller() override
    {
        std::string query = "sum(container_network_transmit_packets_total{job=\"machinesComb\",name!=\"cadvisor\",name!=\"explorer\",name!=\"explorerdb\"})";
        return give_general_response(query);
    }
    rest_response net_var_transmited_per_component_per_minute_controller() override
    {
        std::string query = "sum(rate(container_network_transmit_packets_total{job=\"machinesComb\",name!=\"cadvisor\",name!=\"explorer\",name!=\"explorerdb\"}[1m]))*60";
        return give_general_response(query);
    }
    rest_response net_transmited_per_machine_per_minute_controller() override
    {
        std::string query = "sum(rate(container_network_transmit_packets_total{job=\"machinesComb\",name!=\"cadvisor\",name!=\"explorer\",name!=\"explorerdb\"}[1m]))*60";
        return give_general_response(query);
    }
    /***********************************************************************************/
private:
    rest_response give_general_response(std::string &query)
    {
        //? CAST THE DEPENDENCY INJECTION
        std::shared_ptr<prometheusImpl> implVersion = std::dynamic_pointer_cast<prometheusImpl>(m_prometheus);
        //? CREATE THE HANDLER FOR THIS
        std::function<http::message_generator(http::request<http::string_body>)> handler = [implVersion, query](http::request<http::string_body> req) -> http::message_generator
        {
            std::string query_real = query;
            //? MAKE THE QUERY
            boost::json::object jsonVal = (*implVersion).make_query(req, query_real);
            //? CREATE THE ACTUAL RESPONSE
            std::string json_body = boost::json::serialize(jsonVal);
            http::response<http::string_body> res{http::status::accepted, req.version()};
            res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
            res.set(http::field::content_type, "application/json");
            res.set(http::field::access_control_allow_origin, "*");
            res.set(http::field::access_control_allow_methods, "GET, POST, PUT, DELETE, OPTIONS");
            res.set(http::field::access_control_allow_headers, "Content-Type");
            res.body() = json_body;
            res.prepare_payload();
            res.keep_alive(req.keep_alive());
            return res;
        };
        return handler;
    }
};
#endif