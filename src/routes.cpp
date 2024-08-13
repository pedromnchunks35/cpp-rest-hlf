#include <routes.hpp>
#include "handling.cpp"
#include "controllers.cpp"
class routesImpl : public routes
{
public:
    routesImpl(
        std::string main_route,
        std::shared_ptr<handling> handler,
        std::shared_ptr<controllers> controllers)
        : routes(main_route, handler, controllers)
    {
        //? REGISTER THE m_main_route route
        std::shared_ptr<handlingImpl> impl = std::dynamic_pointer_cast<handlingImpl>(handler);
        std::string route_to_insert = m_main_route;
        RegisterRoutesErrors er = RegisterRoutesErrors::OK;
        (*impl).register_main_route(route_to_insert, er);
        //? USE THEN ER TO LOG LOGS (MAYBE IF IN DEBUF MODE)
        std::string route_to_insert_cpu = "/cpu/";
        RegisterRoutesErrors er_cpu = RegisterRoutesErrors::OK;
        (*impl).register_main_route(route_to_insert_cpu, er_cpu);

        std::string route_to_insert_ram = "/ram/";
        RegisterRoutesErrors er_ram = RegisterRoutesErrors::OK;
        (*impl).register_main_route(route_to_insert_ram, er_ram);

        std::string route_to_insert_disk = "/disk/";
        RegisterRoutesErrors er_disk = RegisterRoutesErrors::OK;
        (*impl).register_main_route(route_to_insert_disk, er_disk);

        std::string route_to_insert_ledger = "/ledger/";
        RegisterRoutesErrors er_ledger = RegisterRoutesErrors::OK;
        (*impl).register_main_route(route_to_insert_ledger, er_ledger);

        std::string route_to_insert_io = "/io/";
        RegisterRoutesErrors er_io = RegisterRoutesErrors::OK;
        (*impl).register_main_route(route_to_insert_io, er_io);

        std::string route_to_insert_net = "/net/";
        RegisterRoutesErrors er_net = RegisterRoutesErrors::OK;
        (*impl).register_main_route(route_to_insert_net, er_net);
    }
    void register_main_routes() override
    {
        std::shared_ptr<handlingImpl> handlingImplementation = std::dynamic_pointer_cast<handlingImpl>(m_handler);
        std::shared_ptr<controllersImpl> controllerImplementation = std::dynamic_pointer_cast<controllersImpl>(m_controllers);
        std::string main_route = m_main_route;
        RouteInfo route_info_main;
        route_info_main.handler = (*controllerImplementation).main_route();
        route_info_main.method = http::verb::get;
        route_info_main.path = "/";
        RegisterRoutesErrors err_info_main = RegisterRoutesErrors::OK;
        (*handlingImplementation).register_sub_route(main_route, route_info_main, err_info_main);
    }
    void register_cpu_routes() override
    {
        //? GETTING THE RESOURCES
        std::shared_ptr<handlingImpl> handlingImplementation = std::dynamic_pointer_cast<handlingImpl>(m_handler);
        std::shared_ptr<controllersImpl> controllerImplementation = std::dynamic_pointer_cast<controllersImpl>(m_controllers);
        std::string main_route = "/cpu/";

        //? REGISTER CPU VARIATION %
        RouteInfo route_info_cpu_variation;
        route_info_cpu_variation.handler = (*controllerImplementation).cpu_variation_perc_controller();
        route_info_cpu_variation.method = http::verb::get;
        route_info_cpu_variation.path = "/variation/component/perc/";
        RegisterRoutesErrors err_info_cpu_variation_component = RegisterRoutesErrors::OK;
        (*handlingImplementation).register_sub_route(main_route, route_info_cpu_variation, err_info_cpu_variation_component);

        //? REGISTER CPU VARIATION PER MACHINE
        RouteInfo route_info_cpu_variation_per_machine;
        route_info_cpu_variation_per_machine.handler = (*controllerImplementation).cpu_variation_per_machine_controller();
        route_info_cpu_variation_per_machine.method = http::verb::get;
        route_info_cpu_variation_per_machine.path = "/variation/machine/perc/";
        RegisterRoutesErrors err_info_cpu_variation_machine = RegisterRoutesErrors::OK;
        (*handlingImplementation).register_sub_route(main_route, route_info_cpu_variation_per_machine, err_info_cpu_variation_machine);
    }
    void register_gb_routes() override
    {
        //? GETTING THE RESOURCES
        std::shared_ptr<handlingImpl> handlingImplementation = std::dynamic_pointer_cast<handlingImpl>(m_handler);
        std::shared_ptr<controllersImpl> controllerImplementation = std::dynamic_pointer_cast<controllersImpl>(m_controllers);
        std::string main_route = "/ram/";

        //? REGISTER RAM GB PER COMPONENT
        RouteInfo route_info_gb_per_component;
        route_info_gb_per_component.handler = (*controllerImplementation).ram_gb_per_component_controller();
        route_info_gb_per_component.method = http::verb::get;
        route_info_gb_per_component.path = "/component/";
        RegisterRoutesErrors err_info_gb_per_component = RegisterRoutesErrors::OK;
        (*handlingImplementation).register_sub_route(main_route, route_info_gb_per_component, err_info_gb_per_component);

        //? REGISTER RAM GB PER MACHINE
        RouteInfo route_info_gb_per_machine;
        route_info_gb_per_machine.handler = (*controllerImplementation).ram_gb_per_machine_controller();
        route_info_gb_per_machine.method = http::verb::get;
        route_info_gb_per_machine.path = "/machine/";

        //? REGISTER RAM PER COMPONENT %
        RouteInfo route_info_gb_per_component_perc;
        route_info_gb_per_component_perc.handler = (*controllerImplementation).ram_gb_per_component_perc_controller();
        route_info_gb_per_component_perc.method = http::verb::get;
        route_info_gb_per_component_perc.path = "/component/perc/";
        RegisterRoutesErrors err_info_gb_per_component_perc = RegisterRoutesErrors::OK;
        (*handlingImplementation).register_sub_route(main_route, route_info_gb_per_component_perc, err_info_gb_per_component_perc);

        //? REGISTER RAM PER MACHINE %
        RouteInfo route_info_gb_per_machine_perc;
        route_info_gb_per_machine_perc.handler = (*controllerImplementation).ram_gb_per_machine_perc_controller();
        route_info_gb_per_machine_perc.method = http::verb::get;
        route_info_gb_per_machine_perc.path = "/machine/perc/";
        RegisterRoutesErrors err_info_gb_per_machine_perc;
        (*handlingImplementation).register_sub_route(main_route, route_info_gb_per_machine_perc, err_info_gb_per_machine_perc);
    }
    void register_disk_usage() override
    {
        //? GETTING THE RESOURCES
        std::shared_ptr<handlingImpl> handlingImplementation = std::dynamic_pointer_cast<handlingImpl>(m_handler);
        std::shared_ptr<controllersImpl> controllerImplementation = std::dynamic_pointer_cast<controllersImpl>(m_controllers);
        std::string main_route = "/disk/";

        //? REGISTER DISK USAGE GB PER COMPONENT
        RouteInfo route_info_disk_usage_per_component;
        route_info_disk_usage_per_component.handler = (*controllerImplementation).disk_usage_per_component_controller();
        route_info_disk_usage_per_component.method = http::verb::get;
        route_info_disk_usage_per_component.path = "/component/";
        RegisterRoutesErrors err_info_disk_usage_per_component = RegisterRoutesErrors::OK;
        (*handlingImplementation).register_sub_route(main_route, route_info_disk_usage_per_component, err_info_disk_usage_per_component);

        //? REGISTER DISK USAGE GB PER MACHINE
        RouteInfo route_info_disk_usage_per_machine;
        route_info_disk_usage_per_machine.handler = (*controllerImplementation).disk_usage_per_machine_controller();
        route_info_disk_usage_per_machine.method = http::verb::get;
        route_info_disk_usage_per_machine.path = "/machine/";
        RegisterRoutesErrors err_info_disk_usage_per_machine = RegisterRoutesErrors::OK;
        (*handlingImplementation).register_sub_route(main_route, route_info_disk_usage_per_machine, err_info_disk_usage_per_machine);

        //? REGISTER DISK USAGE GB PER COMPONENT %
        RouteInfo route_info_disk_usage_per_component_perc;
        route_info_disk_usage_per_component_perc.handler = (*controllerImplementation).disk_usage_per_component_perc_controller();
        route_info_disk_usage_per_component_perc.method = http::verb::get;
        route_info_disk_usage_per_component_perc.path = "/component/perc/";
        RegisterRoutesErrors err_info_disk_usage_per_component_perc = RegisterRoutesErrors::OK;
        (*handlingImplementation).register_sub_route(main_route, route_info_disk_usage_per_component_perc, err_info_disk_usage_per_component_perc);

        //? REGISTER DISK USAGE GB PER MACHINE %
        RouteInfo route_info_disk_usage_per_machine_perc;
        route_info_disk_usage_per_machine_perc.handler = (*controllerImplementation).disk_usage_per_machine_perc_controller();
        route_info_disk_usage_per_machine_perc.method = http::verb::get;
        route_info_disk_usage_per_machine_perc.path = "/machine/perc/";
        RegisterRoutesErrors err_info_disk_usage_per_machine_perc = RegisterRoutesErrors::OK;
        (*handlingImplementation).register_sub_route(main_route, route_info_disk_usage_per_machine_perc, err_info_disk_usage_per_machine_perc);
    }
    void register_ledger() override
    {
        //? GETTING THE RESOURCES
        std::shared_ptr<handlingImpl> handlingImplementation = std::dynamic_pointer_cast<handlingImpl>(m_handler);
        std::shared_ptr<controllersImpl> controllerImplementation = std::dynamic_pointer_cast<controllersImpl>(m_controllers);
        std::string main_route = "/ledger/";

        //? REGISTER LEDGER PER MACHINE GB
        RouteInfo route_info_ledger_gb;
        route_info_ledger_gb.handler = (*controllerImplementation).ledger_per_machine_gb_controller();
        route_info_ledger_gb.method = http::verb::get;
        route_info_ledger_gb.path = "/machine/";
        RegisterRoutesErrors err_info_ledger_gb = RegisterRoutesErrors::OK;
        (*handlingImplementation).register_sub_route(main_route, route_info_ledger_gb, err_info_ledger_gb);

        //? REGISTER LEDGER PER MACHINE GB %
        RouteInfo route_info_ledger_gb_perc;
        route_info_ledger_gb_perc.handler = (*controllerImplementation).ledger_per_machine_gb_perc_controller();
        route_info_ledger_gb_perc.method = http::verb::get;
        route_info_ledger_gb_perc.path = "/machine/perc/";
        RegisterRoutesErrors err_info_ledger_gb_perc = RegisterRoutesErrors::OK;
        (*handlingImplementation).register_sub_route(main_route, route_info_ledger_gb_perc, err_info_ledger_gb_perc);
    }
    void register_io_operations() override
    {
        //? GETTING THE RESOURCES
        std::shared_ptr<handlingImpl> handlingImplementation = std::dynamic_pointer_cast<handlingImpl>(m_handler);
        std::shared_ptr<controllersImpl> controllerImplementation = std::dynamic_pointer_cast<controllersImpl>(m_controllers);
        std::string main_route = "/io/";

        //? NUMBER OF IO OPERATIONS PER COMPONENT
        RouteInfo route_info_io_per_component;
        route_info_io_per_component.handler = (*controllerImplementation).io_number_ops_per_component_controller();
        route_info_io_per_component.method = http::verb::get;
        route_info_io_per_component.path = "/component/";
        RegisterRoutesErrors err_info_io_per_component = RegisterRoutesErrors::OK;
        (*handlingImplementation).register_sub_route(main_route, route_info_io_per_component, err_info_io_per_component);

        //? NUMBER OF IO OPERATIONS PER MACHINE
        RouteInfo route_info_io_per_machine;
        route_info_io_per_machine.handler = (*controllerImplementation).io_number_ops_per_machine_controller();
        route_info_io_per_machine.method = http::verb::get;
        route_info_io_per_machine.path = "/machine/";
        RegisterRoutesErrors err_info_io_per_machine = RegisterRoutesErrors::OK;
        (*handlingImplementation).register_sub_route(main_route, route_info_io_per_machine, err_info_io_per_machine);

        //? VARIATION IO PER MINUTE PER COMPONENT
        RouteInfo route_info_io_per_minute_per_component;
        route_info_io_per_minute_per_component.handler = (*controllerImplementation).io_var_number_of_io_per_minute_per_component_controller();
        route_info_io_per_minute_per_component.method = http::verb::get;
        route_info_io_per_minute_per_component.path = "/variation/component/";
        RegisterRoutesErrors err_info_io_per_minute_per_component = RegisterRoutesErrors::OK;
        (*handlingImplementation).register_sub_route(main_route, route_info_io_per_minute_per_component, err_info_io_per_minute_per_component);

        //? VARIATION IO PER MINUTE PER MACHINE
        RouteInfo route_info_io_per_minute_per_machine;
        route_info_io_per_minute_per_machine.handler = (*controllerImplementation).io_var_number_of_io_per_machine_per_minute_controller();
        route_info_io_per_minute_per_machine.method = http::verb::get;
        route_info_io_per_minute_per_machine.path = "/variation/machine/";
        RegisterRoutesErrors err_info_io_per_minute_per_machine = RegisterRoutesErrors::OK;
        (*handlingImplementation).register_sub_route(main_route, route_info_io_per_minute_per_machine, err_info_io_per_minute_per_machine);

        //? READS PER COMPONENT
        RouteInfo route_info_reads_component;
        route_info_reads_component.handler = (*controllerImplementation).io_number_of_reads_per_component_controller();
        route_info_reads_component.method = http::verb::get;
        route_info_reads_component.path = "/reads/component/";
        RegisterRoutesErrors err_info_reads_component = RegisterRoutesErrors::OK;
        (*handlingImplementation).register_sub_route(main_route, route_info_reads_component, err_info_reads_component);

        //? READS PER MACHINE
        RouteInfo route_info_reads_machine;
        route_info_reads_machine.handler = (*controllerImplementation).io_number_of_reads_per_machine_controller();
        route_info_reads_machine.method = http::verb::get;
        route_info_reads_machine.path = "/reads/machine/";
        RegisterRoutesErrors err_info_reads_machine = RegisterRoutesErrors::OK;
        (*handlingImplementation).register_sub_route(main_route, route_info_reads_machine, err_info_reads_machine);

        //? VARIATION READS PER MINUTE PER COMPONENT
        RouteInfo route_info_reads_per_minute_component;
        route_info_reads_per_minute_component.handler = (*controllerImplementation).io_var_number_of_reads_per_minute_per_component_controller();
        route_info_reads_machine.method = http::verb::get;
        route_info_reads_machine.path = "/variation/reads/component/";
        RegisterRoutesErrors err_info_reads_variation_component = RegisterRoutesErrors::OK;
        (*handlingImplementation).register_sub_route(main_route, route_info_reads_per_minute_component, err_info_reads_variation_component);

        //? VARIATION READS PER MINUTE PER MACHINE
        RouteInfo route_info_reads_per_minute_machine;
        route_info_io_per_minute_per_machine.handler = (*controllerImplementation).io_var_number_cumulative_reads_per_machine_per_minute_controller();
        route_info_io_per_minute_per_machine.method = http::verb::get;
        route_info_io_per_minute_per_machine.path = "/variation/reads/machine/";
        RegisterRoutesErrors err_info_reads_variation_machine = RegisterRoutesErrors::OK;
        (*handlingImplementation).register_sub_route(main_route, route_info_reads_per_minute_machine, err_info_reads_variation_machine);

        //? WRITES PER COMPONENT
        RouteInfo route_info_writes_per_component;
        route_info_writes_per_component.handler = (*controllerImplementation).io_number_writes_per_component_controller();
        route_info_writes_per_component.method = http::verb::get;
        route_info_writes_per_component.path = "/writes/component/";
        RegisterRoutesErrors err_info_writes_per_component = RegisterRoutesErrors::OK;
        (*handlingImplementation).register_sub_route(main_route, route_info_writes_per_component, err_info_writes_per_component);

        //? WRITES PER MACHINE
        RouteInfo route_info_writes_per_machine;
        route_info_writes_per_machine.handler = (*controllerImplementation).io_number_writes_per_machine_controller();
        route_info_writes_per_machine.method = http::verb::get;
        route_info_writes_per_machine.path = "/writes/machine/";
        RegisterRoutesErrors err_info_writes_per_machine = RegisterRoutesErrors::OK;
        (*handlingImplementation).register_sub_route(main_route, route_info_writes_per_machine, err_info_writes_per_machine);

        //? VARIATION WRITES PER COMPONENT
        RouteInfo route_info_var_writes_per_component;
        route_info_var_writes_per_component.handler = (*controllerImplementation).io_var_number_writes_per_minute_per_component_controller();
        route_info_var_writes_per_component.method = http::verb::get;
        route_info_var_writes_per_component.path = "/variation/writes/component/";
        RegisterRoutesErrors err_info_var_writes_per_component = RegisterRoutesErrors::OK;
        (*handlingImplementation).register_sub_route(main_route, route_info_var_writes_per_component, err_info_var_writes_per_component);

        //? VARIATION WRITES PER MACHINE
        RouteInfo route_info_var_writes_per_machine;
        route_info_var_writes_per_machine.handler = (*controllerImplementation).io_number_writes_per_machine_per_minute_controller();
        route_info_var_writes_per_machine.method = http::verb::get;
        route_info_var_writes_per_machine.path = "/variation/writes/machine/";
        RegisterRoutesErrors err_info_var_writes_per_machine = RegisterRoutesErrors::OK;
        (*handlingImplementation).register_sub_route(main_route, route_info_var_writes_per_machine, err_info_var_writes_per_machine);
    }
    void register_network_packets() override
    {
        //? GETTING THE RESOURCES
        std::shared_ptr<handlingImpl> handlingImplementation = std::dynamic_pointer_cast<handlingImpl>(m_handler);
        std::shared_ptr<controllersImpl> controllerImplementation = std::dynamic_pointer_cast<controllersImpl>(m_controllers);
        std::string main_route = "/net/";
        //? RECEIVED PACKETS PER COMPONENT
        RouteInfo route_info_received_packets_component;
        route_info_received_packets_component.handler = (*controllerImplementation).net_number_received_per_component_controller();
        route_info_received_packets_component.method = http::verb::get;
        route_info_received_packets_component.path = "/received/component/";
        RegisterRoutesErrors err_info_received_packets_component = RegisterRoutesErrors::OK;
        (*handlingImplementation).register_sub_route(main_route, route_info_received_packets_component, err_info_received_packets_component);

        //? RECEIVED PACKETS PER MACHINE
        RouteInfo route_info_received_machine;
        route_info_received_machine.handler = (*controllerImplementation).net_number_io_received_per_machine_controller();
        route_info_received_machine.method = http::verb::get;
        route_info_received_machine.path = "/received/machine/";
        RegisterRoutesErrors err_info_received_machine = RegisterRoutesErrors::OK;
        (*handlingImplementation).register_sub_route(main_route, route_info_received_machine, err_info_received_machine);

        //? VARIATION RECEIVED COMPONENT
        RouteInfo route_info_var_received_component;
        route_info_var_received_component.handler = (*controllerImplementation).net_var_received_per_minute_per_component_controller();
        route_info_var_received_component.method = http::verb::get;
        route_info_var_received_component.path = "/variation/received/component/";
        RegisterRoutesErrors err_info_var_received_component = RegisterRoutesErrors::OK;
        (*handlingImplementation).register_sub_route(main_route, route_info_var_received_component, err_info_var_received_component);

        //? VARIATION RECEIVED MACHINE
        RouteInfo route_info_var_received_machine;
        route_info_var_received_machine.handler = (*controllerImplementation).net_number_io_received_per_machine_per_minute_controller();
        route_info_var_received_machine.method = http::verb::get;
        route_info_var_received_machine.path = "/variation/received/machine/";
        RegisterRoutesErrors err_info_var_received_machine = RegisterRoutesErrors::OK;
        (*handlingImplementation).register_sub_route(main_route, route_info_var_received_machine, err_info_var_received_machine);

        //? TRANSMITED COMPONENTS
        RouteInfo route_info_transmited_component;
        route_info_transmited_component.handler = (*controllerImplementation).net_number_transmited_per_component_controller();
        route_info_transmited_component.method = http::verb::get;
        route_info_transmited_component.path = "/transmited/component/";
        RegisterRoutesErrors err_info_transmited_component = RegisterRoutesErrors::OK;
        (*handlingImplementation).register_sub_route(main_route, route_info_transmited_component, err_info_transmited_component);

        //? TRANSMITED MACHINE
        RouteInfo route_info_transmited_machine;
        route_info_transmited_machine.handler = (*controllerImplementation).net_transmited_per_machine_controller();
        route_info_transmited_machine.method = http::verb::get;
        route_info_transmited_machine.path = "/transmited/machine/";
        RegisterRoutesErrors err_info_transmited_machine = RegisterRoutesErrors::OK;
        (*handlingImplementation).register_sub_route(main_route, route_info_transmited_machine, err_info_transmited_machine);

        //? VARIATION TRANSMITED COMPONENTS
        RouteInfo route_info_variation_transmited_component;
        route_info_variation_transmited_component.handler = (*controllerImplementation).net_var_transmited_per_component_per_minute_controller();
        route_info_variation_transmited_component.method = http::verb::get;
        route_info_variation_transmited_component.path = "/variation/transmited/component/";
        RegisterRoutesErrors err_info_variation_transmited_component = RegisterRoutesErrors::OK;
        (*handlingImplementation).register_sub_route(main_route, route_info_variation_transmited_component, err_info_variation_transmited_component);

        //? VARIATION TRANSMITED MACHINE
        RouteInfo route_info_variation_transmited_machine;
        route_info_variation_transmited_machine.handler = (*controllerImplementation).net_transmited_per_machine_per_minute_controller();
        route_info_variation_transmited_machine.method = http::verb::get;
        route_info_variation_transmited_machine.path = "/variation/transmited/machine/";
        RegisterRoutesErrors err_info_variation_transmited_machine = RegisterRoutesErrors::OK;
        (*handlingImplementation).register_sub_route(main_route, route_info_variation_transmited_machine, err_info_variation_transmited_machine);
    }
};