# Proxy Server in C

This project is a proxy server written in C that:

* **Listens for IPv4 client connections** on port 22126.
* **Responds to specific commands** ( 01#) by connecting to an IPv6 server, fetching content (an HTML page), and:
    * Sending it back to the IPv4 client.
    * Saving it to a local file (pagina.html).
    * Displaying it in the server's terminal.

## Features

* **IPv4 and IPv6 Support:** Uses sockets for both protocols to communicate between clients and servers.
* **HTTP Protocol Handling:** Sends HTTP GET requests and processes responses from the IPv6 endpoint.
* **Command-Based Interface:** Responds to specific text commands received from the client.
* **Error Management:** Provides meaningful error messages and ensures cleanup of resources.
* **File Logging:** Saves the fetched content to a file (pagina.html).


