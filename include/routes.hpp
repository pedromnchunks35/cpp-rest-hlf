#ifndef ROUTES
#define ROUTES
#include <iostream>
#include "handling.hpp"
#include "controllers.hpp"
class routes
{
protected:
    std::string m_main_route;
    std::shared_ptr<handling> m_handler;
    std::shared_ptr<controllers> m_controllers;

public:
    explicit routes(
        std::string main_route,
        std::shared_ptr<handling> handler,
        std::shared_ptr<controllers> controllers)
        : m_handler(handler),
          m_main_route(main_route),
          m_controllers(std::move(controllers))
    {
    }
    virtual void register_main_routes() = 0;
    virtual void register_cpu_routes() = 0;
    virtual void register_gb_routes() = 0;
    virtual void register_disk_usage() = 0;
    virtual void register_ledger() = 0;
    virtual void register_io_operations() = 0;
    virtual void register_network_packets() = 0;
};
#endif