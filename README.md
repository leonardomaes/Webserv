# Webserv

*This project has been created as part of the 42 curriculum by lmaes and rda-cunh.*

## Description

**Webserv** is a fully functional HTTP/1.1 web server implemented in C++98. The goal of this project is to understand the underlying mechanisms of the internet by building a server from scratch, without relying on heavy external frameworks like NGINX or Apache.

This server utilizes non-blocking I/O operations with `epoll` to handle multiple client connections simultaneously. It supports standard HTTP methods (GET, POST, DELETE), processes static and dynamic content (via CGI), handles file uploads, and manages server configurations through a customizable file (.conf). It serves as a deep dive into network programming, socket manipulation, and the HTTP protocol specifications (RFC 7230-7235).

## Features

- **HTTP/1.1 Support:** Comply with major HTTP standards.
- **I/O Multiplexing:** Uses `epoll` for efficient, non-blocking event handling.
- **Method Support:** `GET`, `POST`, and `DELETE`.
- **CGI Execution:** Support for dynamic scripts (Python and PHP) with environment variable handling (`PATH_INFO`, `QUERY_STRING`, `COOKIES`) and timeout protection.
- **Configuration Parsing:** Custom `.conf` files to define servers, ports, routes, and rules, inspired by NGINX.
- **Directory Listing:** Auto-indexing feature for directories.
- **File Uploads:** Handles `multipart/form-data` and binary file saving.
- **Client State:** Cookie management support.
- **UX/UI Webpage Interface for tests:** Includes a user oriented interface (index.html) for better testing.

## Instructions

### Requirements
- Linux / Unix environment
- C++98 compiler (g++)
- `make`
- Python3 (for Python CGI scripts)
- `php-cgi` (for PHP CGI scripts)

### Compilation
To compile the project, run the following command at the root of the repository:

```bash
make
```

This will generate the `webserv` executable.

### Execution
Run the server by providing a configuration file. If no file is provided, it defaults to `default.conf`.

```bash
./webserv [configuration_file]
```

**Example:**
```bash
./webserv default.conf
```

### Testing
Once the server is running (default port `8080`), you can test it using a web browser or `curl`:

1.  **Browser:** Open `http://localhost:8080` to see the index page.
2.  **CGI Tests:** Navigate to the CGI section to test Python and PHP scripts.
3.  **Uploads:** Use the provided HTML forms to upload and delete files to the server.

## Resources

This project was built on the foundation of the following excellent resources:

### Networking & Sockets (The Foundation)
- **[Beej's Guide to Network Programming](https://beej.us/guide/bgnet/)** - The definitive guide for socket programming in C. **Start here.**
- **[Sockets and Network Programming in C](https://www.codequoi.com/en/sockets-and-network-programming-in-c)** - A clear introduction.
- **[Handling multiple clients without multithreading](https://www.geeksforgeeks.org/socket-programming-in-cc-handling-multiple-clients-on-server-without-multi-threading/)** - For understanding the `select`/`poll`/`epoll` model.
- **[EPOLL Explanation (Video)](https://www.youtube.com/watch?v=WuwUk7Mk80E)** - A visual guide to understanding I/O multiplexing.

### HTTP Protocol & Server Logic
- **[Mozilla MDN: HTTP Overview](https://developer.mozilla.org/en-US/docs/Web/HTTP)** - The standard for web documentation.
- **[RFC 7230 (HTTP/1.1 Syntax and Routing)](https://datatracker.ietf.org/doc/html/rfc7230)** - The official rules.
- **[Building your own Web Server (Series)](https://dev.to/dmytro_huz/building-your-own-web-server-part-1-theory-and-foundations-3kgo)** - A comprehensive walkthrough of the theory and implementation.

### Configuration & NGINX
- **[NGINX Beginner's Guide](https://nginx.org/en/docs/beginners_guide.html)** - Used as a reference for our config file structure.
- **[Understanding Nginx Configuration Contexts](https://www.digitalocean.com/community/tutorials/understanding-the-nginx-configuration-file-structure-and-configuration-contexts)** - Helped in structuring our parsing logic.

### AI Usage
Artificial Intelligence tools were used to assist with the following tasks during development:
- **Debugging & Logic Analysis:** Analyzing complex logic flows in `Response.cpp` and `Request.cpp` to identify root causes of bugs (e.g., path parsing issues, infinite loops in CGI).
- **Test Script Generation:** Creating robust Python and PHP scripts to validate specific server features like cookies, infinite loop timeouts, and image rendering.
- **Code Optimization:** Suggesting improvements for non-blocking I/O handling and standard compliance (e.g., proper `select` usage for timeouts).
- **Documentation:** Assisting in structuring and drafting project documentation and the README file.

---