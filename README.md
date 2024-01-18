# 42Webserv
`42 Webserv` is a lightweight HTTP server implemented in C++ 98, designed to handle various HTTP methods and configurations. The server employs non-blocking I/O using the select() system call for efficient communication with clients. It supports features such as serving static websites, handling GET, POST, and DELETE methods, as well as supporting file uploads

<img src="https://dl.dropbox.com/scl/fi/gozmten888oibjlxh321x/webserv.png?rlkey=qa3tts18lxi562eczgu1p4dyg&dl=0"></img>

## Table of contents
- [Socket](Socket Programming)

## Socket
HTTP communication usually takes place over TCP. A typical HTTP session often consists of three steps: The client and server establish a TCP connection stream, the client sends HTTP request over TCP connection, and then the server processes that request and sends back a reply. The second and third step can be repeated any number of times, until both client and server decide to close the underlying TCP connection. To put it in a simple diagram, this is how the process looks like in the perspective of TCP.

<img src="https://dl.dropbox.com/scl/fi/vlfsd1xpsev03z2q9j0gl/socket.png?rlkey=iueejqzkr0stqa2q2m27yx5jw&dl=0">

