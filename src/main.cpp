#include "connection.cpp"
#include "handling.cpp"
#include "prometheus.cpp"
#include "controllers.cpp"
#include "routes.cpp"
#include <thread>
#include <vector>
#include <debug.hpp>
int main()
{
  LOG_W("Debug mode is activated");
  //? CREATE THE BASIC CONFIGS FOR NETWORKING
  net::ip::address_v4 const address = net::ip::address_v4::loopback();
  int const port = 80;
  std::shared_ptr<std::string> const doc_root = std::make_shared<std::string>(".");
  int const threads = 5;
  net::io_context ioc{5};

  //? CREATE OUR HANDLING OBJECT
  handlingImpl handler;
  uint8_t size_main_route = 1;
  handler.init(size_main_route);
  std::shared_ptr<handlingImpl> handlerWrapped = std::make_shared<handlingImpl>(handler);

  //? CREATE PROMETHEUS INSTANCE
  std::string hostname_prometheus = "localhost";
  std::string port_prometheus = "9090";
  std::string endpoint_prometheus = "/api/v1/query_range";
  prometheusImpl p_impl = prometheusImpl(hostname_prometheus, port_prometheus, endpoint_prometheus);
  std::shared_ptr<prometheusImpl> p_impl_wrapped = std::make_shared<prometheusImpl>(p_impl);

  //? CREATE THE CONTROLLER INSTANCE
  controllersImpl c_impl = controllersImpl(p_impl_wrapped);
  std::shared_ptr<controllersImpl> c_impl_wrapped = std::make_shared<controllersImpl>(c_impl);

  //? CREATE THE ROUTES REGISTERING OBJECT
  std::string main_route = "/";
  routesImpl r_impl = routesImpl(main_route, handlerWrapped, c_impl_wrapped);
  r_impl.register_main_routes();
  r_impl.register_cpu_routes();
  r_impl.register_disk_usage();
  r_impl.register_gb_routes();
  r_impl.register_io_operations();
  r_impl.register_ledger();
  r_impl.register_network_packets();

  //? MAKE IT RUN
  std::make_shared<listenerImpl>(
      ioc,
      tcp::endpoint{address, port},
      doc_root,
      handlerWrapped)
      ->run();

  //? RUN THE THREADS
  std::vector<std::thread> v;
  v.reserve(threads - 1);
  for (int i = threads - 1; i > 0; --i)
  {
    v.emplace_back(
        [&ioc]
        {
          ioc.run();
        });
  }

  //? run main thread otherwise it will not keep up
  std::cout << "Listening the application at 0.0.0.0:" << port << "\n";
  ioc.run();
  return EXIT_SUCCESS;
}