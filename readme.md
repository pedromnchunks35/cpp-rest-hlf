# INFO
- This is supposed to be a rest service to wrap data from our prometheus
# ROUTES
- CPU
  - CPU VARIATION % (PER COMPONENTS)
  - CPU VARIATION % (PER MACHINE)
- RAM GB
  - RAM GB PER COMPONENT
  - RAM GB PER MACHINE
  - RAM GB PER COMPONENT %
  - RAM GB PER MACHINE %
- DISK USAGE
  - DISK USAGE GB PER COMPONENT
  - DISK USAGE GB PER MACHINE
  - DISK USAGE % PER COMPONENT
  - DISK USAGE PER MACHINE (%)
- LEDGER
  - LEDGER SIZE PER MACHINE GB
  - LEDGER SIZE PER MACHINE GB (%)
- IO OPERATIONS 
  - NUMBER IO OPERATIONS PER COMPONENT
  - NUMBER IO OPERATIONS PER MACHINE
  - VARIATION OF NUMBER OF IO PER MINUTE PER COMPONENT
  - VARIATION OF NUMBEER OF IO PER MACHINE PER COMPONENT
  - NUMBER OF IO CUMULATIVE READS PER COMPONENT
  - NUMBER OF IO CUMULATIVE READS PER MACHINE
  - VARIATION OF NUMBER OF CUMULATIVE READS PER MINUTE PER COMPONENT
  - NUMBER OF IO CUMULATIVE READ OPERATIONS PER MACHINE PER MINUTE
  - NUMBER OF IO CUMULATIVE WRITES PER COMPONENT
  - NUMBER OF IO CUMULATIVE WRITE OPERATIONS PER MACHINE
  - VARIATION OF NUMBER OF CUMULATIVE WRITES PER MINUTE PER COMPONENT
  - NUMBER OF IO CUMULATIVE WRITE OPERATIONS PER MACHINE PER MINUTE
- NETWORK PACKETS
  - NUMBER OF CUMULATIVE RECEIVED NETWORK PACKETS
  - NUMBER OF IO CUMULATIVE RECEIVED NETWORK PACKETS PER MACHIN
  - VARIATION OF NUMBER OF CUMULATIVE RECEIVED PACKETS NETWORK PER MINUTE PER COMPONENT
  - NUMBER OF IO CUMULATIVE RECEIVED PACKETS NETWORK PER MACHINE PER MINUTE
  - NUMBER OF CUMULATIVE TRANSMITED NETWORK PACKETS
  - NUMBER OF IO CUMULATIVE TRANSMITED NETWORK PACKETS PER MACHINE
  - VARIATION OF NUMBER OF CUMULATIVE TRANSMITED PACKETS NETWORK PER MINUTE PER COMPONENT
  - NUMBER OF IO CUMULATIVE TRANSMITED PACKETS NETWORK PER MACHINE PER MINUTE
# Header files
1. connection.hpp
- Purpose: Manage sessions and listen for REST service requests.
- Typical Contents:
  - Definitions for managing HTTP sessions.
  - Handling incoming connections and requests.
  - Possibly defining the server-side aspects of the HTTP connection, such as starting and stopping the server.
2. controllers.hpp
- Purpose: Define the controllers that process requests and generate responses.
- Typical Contents:
  - Interface definitions for controllers with methods like cpuController(), ramGbController(), etc.
  - Methods typically return a RestResponse or similar type that encapsulates the HTTP response.
3. handling.hpp
- Purpose: Handle request dispatching and routing.
- Typical Contents:
  - Definitions for how incoming requests are routed to the appropriate controllers.
    Handling the parsing of request data and invoking the correct controller methods.
    Considerations: Implement a flexible routing mechanism that can handle various HTTP methods and paths. Ensure that request handling is efficient and supports the expected request patterns of your application.
4. prometheus.hpp
- Purpose: Manage interactions with Prometheus, such as querying metrics.
- Typical Contents:
  - Interface definitions for querying Prometheus.
  - Methods for building and sending requests to Prometheus and handling responses.
5. Dependency Injection and Testability
- Purpose: Enable the injection of dependencies and facilitate testing.
