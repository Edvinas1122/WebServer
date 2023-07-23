# WebServer
The goal of this project was to create a web server in C++, similar to nginx.

## About the Project

The WebServer project is a high-performance, scalable web server implemented in C++. It's similar to nginx, featuring a configuration file parser, and HTTP request/response handling mechanisms.

Key features of this web server include:

- **Poll and Sockets:** The web server utilizes the poll system call for I/O multiplexing, and BSD sockets for network communications. This allows the server to handle multiple connections simultaneously.
  
- **Configuration File Parser:** The server includes a custom configuration file parser. This allows for flexible server configuration, including specifying the listening ports, defining server blocks, and setting default error pages.

- **SSL Support:** The server has SSL support, allowing it to handle HTTPS requests.

- **Experimental WebSocket Support:** The server includes experimental support for the WebSocket protocol.
