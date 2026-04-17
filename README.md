# IPv4-to-IPv6-Proxy-Server


A specialized proxy server developed in C that acts as a bridge between IPv4 clients and IPv6 web servers. The server listens for specific command-based triggers to fetch, display, and log web content.

## Key Features

* **Dual-Stack Connectivity:** Seamless communication between IPv4 (client-side) and IPv6 (server-side) protocols using C sockets.
* **Command-Based Interface:** Responds to custom control sequences (e.g., `01#` for data fetch).
* **HTTP Protocol Handling:** Manages raw HTTP GET requests and processes responses from IPv6 endpoints.
* **Multi-Output Logging:** * Streams fetched content back to the **IPv4 client**.
    * Displays real-time data in the **server terminal**.
    * Persists data locally in `pagina.html`.


