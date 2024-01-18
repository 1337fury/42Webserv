# 42Webserv
`42 Webserv` is a lightweight HTTP server implemented in C++ 98, designed to handle various HTTP methods and configurations. The server employs non-blocking I/O using the select() system call for efficient communication with clients. It supports features such as serving static websites, handling GET, POST, and DELETE methods, as well as supporting file uploads

<img src="https://dl.dropbox.com/scl/fi/gozmten888oibjlxh321x/webserv.png?rlkey=qa3tts18lxi562eczgu1p4dyg&dl=0"></img>

## Table of contents
- [Socket](#Socket Programming)

## Socket
HTTP communication usually takes place over TCP. A typical HTTP session often consists of three steps: The client and server establish a TCP connection stream, the client sends HTTP request over TCP connection, and then the server processes that request and sends back a reply. The second and third step can be repeated any number of times, until both client and server decide to close the underlying TCP connection. To put it in a simple diagram, this is how the process looks like in the perspective of TCP.

<img src="https://dl.dropbox.com/scl/fi/vlfsd1xpsev03z2q9j0gl/socket.png?rlkey=iueejqzkr0stqa2q2m27yx5jw&dl=0">

To create a server you need to follow this steps:
1. Create a socket and listen for new connections.
1. Accept incoming client connections.
1. Receive messages, process them and sends some responses to the client. This is where HTTP message exchange happens.
1. When one party wants to close the connection, it will do that by sending an EOF character and closing the socket file descriptor.

In order to get a good start with socket programming, I recommend this guide <a href="https://beej.us/guide/bgnet/html/#client-server-background">Beej’s Guide to Network Programming</a>

## Http messages
In a client-server setting, `HTTP messages` are the requests and responses objects exchanged between the two parties. An HTTP client sends a HTTP request to an HTTP server, and the server will reply with an HTTP response. The messages must follow some format specified in the RFCs. For the small scope of my project, I picked out the most basic components to implement in my program. In short, an HTTP message should consist of:

* A start line: For an HTTP request, this line includes an HTTP method (GET, POST, HEAD, etc), a request target (URI), and a string that indicates the HTTP version (e.g HTTP/1.1). For an HTTP response, the start line (or status line) will have the HTTP version that the server used, a status code, an optionally, a message describing the status code. The start line of a message should be terminated by a CRLF character.
* Header fields: A list of key - value pairs that appear right after the start line and contain metadata about the HTTP connection and message. Each field should be on a single line and have the format field-name: field-value
* Message body: An optional sequence of bytes. The message body is often present in response messages from the server, and sometimes in requests sent by the client, depending on the HTTP method. An HTTP message body can have any format, as long as both client and server have no issue understanding it.
  
Some examples of HTTP messages:
**HTTP Request**
```
GET /index.html HTTP/1.1
Host: example.com
Connection: keep-alive
User-Agent: Mozilla/5.0
```

