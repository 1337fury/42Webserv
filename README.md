# 42Webserv
`42 Webserv` is a lightweight HTTP server implemented in C++ 98, designed to handle various HTTP methods and configurations. The server employs non-blocking I/O using the select() system call for efficient communication with clients. It supports features such as serving static websites, handling GET, POST, and DELETE methods, as well as supporting file uploads. <br /><br />Teammates with <a href="https://github.com/HChairiRuya" style="color: blue;"><b>@hchairi </b></a>

<img src="https://dl.dropbox.com/scl/fi/gozmten888oibjlxh321x/webserv.png?rlkey=qa3tts18lxi562eczgu1p4dyg&dl=0"></img>

## Table of contents
- [42Webserv](#42webserv)
	- [Socket](#socket)
	- [Http messages](#http-messages)
	- [Http Request](#http-request)
	- [My Request Parser](#my-request-parser)
	- [Http Response](#http-response)
	- [CGI Program](#cgi-program)
	- [Cookies](#cookies)
	- [Resources](#resources)

<a name="Socket"></a>

## Socket
HTTP communication usually takes place over TCP. A typical HTTP session often consists of three steps: The client and server establish a TCP connection stream, the client sends HTTP request over TCP connection, and then the server processes that request and sends back a reply. The second and third step can be repeated any number of times, until both client and server decide to close the underlying TCP connection. To put it in a simple diagram, this is how the process looks like in the perspective of TCP.

<img src="https://dl.dropbox.com/scl/fi/vlfsd1xpsev03z2q9j0gl/socket.png?rlkey=iueejqzkr0stqa2q2m27yx5jw&dl=0"></img>

To create a server you need to follow this steps:
1. Create a socket and listen for new connections.
1. Accept incoming client connections.
1. Receive messages, process them and sends some responses to the client. This is where HTTP message exchange happens.
1. When one party wants to close the connection, it will do that by sending an EOF character and closing the socket file descriptor.

In order to get a good start with socket programming, I recommend this guide <a href="https://beej.us/guide/bgnet/html/#client-server-background">Beej’s Guide to Network Programming</a>

<a name="Http_messages"></a>

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

**HTTP Response**
```
HTTP/1.1 200 OK
Server: Hello
Content-Length: 13
Content-Type: text/plain

Hello, world
```

The classes that represent HTTP messages in my program looks as follows:

**Request class**
```C++
class Request
{
	public:
	// constructors
		Request( void );
		Request( Request const &rhs );
		Request &operator=( Request const &rhs );
		~Request( void );

	// Properties
		std::string				query_string;
		std::string				fragment;
		std::string				method;
		std::string				uri;
		int 					versionMajor;
		int 					versionMinor;
		std::vector<Header> 	headers;
		std::vector<char> 		content;
		bool 					keepAlive;
	//...
};
```

**Response class**
```C++
class Response
{
	private:
	// Properties
		Server					_server;
		u_short					_statusCode;
		std::string				_status;
		std::vector<Header> 	_headers;
		std::vector<char> 		_content;
		std::string				_body;
		Request					_request;
		bool					_keepAlive;
		std::string				_version;
		
		std::string				_response_string;
		
		std::stringstream		_page;
		
		Location				*_location;
		bool					_error;
		pid_t					_cgi_pid;
		int						_cgi_stdout;
		int						_cgi_stderr;
		bool					_cgi;

	public:
	//...
};
```

<a name="Http_request"></a>

## Http Request

An HTTP request is essentially a message your app (like a browser) sends to a server asking for something (like a website page). Think of it like ordering pizza: you tell the server what you want (website address) and they send it back (page content).
> [!NOTE]
> The most important part of an HTTP request is the Request line. Let's discuss this further below.

**Request line**
The first line of an HTTP request is called the Request line. It consists of 4 parts:

```
 GET    /index.html      HTTP/1.1           \r\n
 \_/    \_________/      \______/          \____/
  |          |               |                |
 Method     URI        HTTP version       Line break
```

* `Method` tells the server what action the client wants to perform on the URI. For example, GET method means the client wants the server to send him the page at the given URI. The POST method means the client wants to send some data to the server at the given URI.
* `URI` stands for Uniform Resource Identifier. This tells the server on which resource or page or anything else the client wants to perform the request.
* `HTTP` version is the version of HTTP the client supports or wants the server to use for the response. The most widely used version of HTTP is 1.1.
* `Line break` - this tells the server that the request line has ended and the request headers follow after this line.

## My Request Parser

My parser follows the structure of an HTTP request, distinguishing different parts such as the request method, `URI`, `HTTP version`, `headers`, and `request body`. It uses a state machine with various states to keep track of the parsing progress.

The method performs character-by-character analysis, ensuring that the input adheres to the HTTP request format. It populates the request object with relevant information, such as the request method, URI, headers, and content.

If the parsing is successful and the entire request is processed, the method returns `PARSE_SUCCESS`. If the parsing is incomplete and more data is required to complete the request, it returns `PARSE_INCOMPLETE`. If an error is encountered during parsing, it returns `PARSE_ERROR`.

```C++
enum ParseResult
{
	PARSE_SUCCESS,
	PARSE_ERROR,
	PARSE_INCOMPLETE
};

enum RequestState
{
	REQUEST_METHOD_START,
	REQUEST_METHOD,
	URI_START,
	URI,
	QUERY_STRING_START,
	QUERY_STRING,
	FRAGMENT_START,
	FRAGMENT,
	HTTP_VERSION_H,
	HTTP_VERSION_HT,
	HTTP_VERSION_HTT,
	HTTP_VERSION_HTTP,
	HTTP_VERSION_SLASH,
	HTTP_VERSION_MAJOR_START,
	HTTP_VERSION_MAJOR,
	HTTP_VERSION_MINOR_START,
	HTTP_VERSION_MINOR,
	HTTP_VERSION_CR,

	HEADER_LINE_START,
	HEADER_LWS,
	HEADER_KEY,
	SPACE_BEFORE_HEADER_VALUE,
	HEADER_VALUE,
	EXPECTING_NEWLINE_2,
	EXPECTING_NEWLINE_3,

	POST_BODY,
	CHUNKED_BODY_SIZE,
	CHUNKED_SIZE_NEWLINE,
	CHUNKED_SIZE_NEWLINE_2,
	CHUNKED_SIZE_NEWLINE_3,
	
	CHUNKED_DATA_NEWLINE,
	CHUNKED_DATA_NEWLINE_2,
	CHUNKED_DATA
};
```

<a name="Http_response"></a>

## Http Response

An HTTP response is the server's reply to your request (like when you open a website). It carries the requested information (webpage, data) and status codes (success, error) like a waiter bringing your order and telling you if it went well. Think of it as the other half of the conversation with the server!

My webserver builds the appropriate HTTP response based on the return value from the HTTP request analyzer. The analyzeRequest method examines key elements of the incoming request, such as the `request method`, `URI`, `headers`, and `content size`, to determine the appropriate response status. The response status is then used to guide the creation of the corresponding HTTP response. This ensures that the server follows the HTTP protocol standards, responding with status codes like `404 (Not Found)`, `405 (Method Not Allowed)`, or `413 (Request Entity Too Large)` when necessary. Additionally, the server handles specific cases, such as redirects or file uploads, by generating responses tailored to the analyzed request. By aligning the response with the nature of the request, the webserver effectively communicates with clients, providing accurate and meaningful feedback about the success or failure of their requests.

```C++
enum reqStatus
{
	LOCATION_NOT_FOUND,
	LOCATIONS_IS_REDIRECTING,
	LOCATION_IS_UPLOADING,
	METHOD_NOT_ALLOWED,
	REQUEST_TO_LARGE,
	PATH_NOT_EXISTING,
	PATH_IS_DIRECTORY,
	PATH_IS_FILE,
	OK
};

reqStatus				analyzeRequest( std::string &path );
```

<a name="cgi_program"></a>

## CGI Program

`CGI` (Common Gateway Interface) is a way for web servers and server-side programs to interact. CGI is completely independent of programming language, operating system and web server. Currently it is the most common server-side programming technique and it's also supported by almost every web server in existence. Moreover, all servers implement it in (nearly) the same way, so that you can make a CGI script for one server and then distribute it to be run on any web server.

The server needs a way to know which URLs map to scripts and which URLs just map to ordinary HTML files. For my CGI i start by creating CGI directories on the server. This is done in the server setup and tells the server that all files in a cgi-bin directory are CGI scripts to be executed when requested. so one can tell that URLs like this: `http://localhost/cgi-bin/anim.js` point to a CGI script.

<img src="https://dl.dropbox.com/scl/fi/a69e7swor5gnvluf9w12f/CGI-1.png?rlkey=jb9n1wp1k3ostvm5ph3o6veka&dl=0"></img>

More information on CGI is available here <a href="http://www.mnuwer.dbasedeveloper.co.uk/dlearn/web/session01.htm">Getting Started with CGI Programs</a>

<a name="cookies"></a>

## Cookies

`HTTP cookies` are small pieces of data stored on a user's device by web browsers to retain information about their interactions with a website. This information, in the form of key-value pairs, is sent between the client and the server with each HTTP request. Cookies serve various purposes, such as session management, user authentication, and personalization. For instance, they can store user preferences, track login sessions, or remember items in a shopping cart. Cookies are essential for creating a personalized and seamless user experience by allowing websites to recognize and remember users across visits.
Additionally, my server incorporates a straightforward HTTP cookie mechanism, enabling basic functionalities like simple user state management.

<img src="https://dl.dropbox.com/scl/fi/sr99fsigr0i0qbsq3eoc6/cookies.png?rlkey=1agghnphfsw25944z3mk7zfu9&dl=0"></img>

https://github.com/1337fury/42Webserv/assets/107589431/091cdcaa-33a8-4ec3-8497-c54ddf8ddf02

<a name="resources"></a>

## Resources

<a href="https://developer.mozilla.org/en-US/docs/Web/HTTP/Session">HTTP Session</a> <br>
<a href="https://www.digitalocean.com/community/tutorials/understanding-nginx-server-and-location-block-selection-algorithms">Nginx Server and Location</a> <br>
<a href="https://osasazamegbe.medium.com/showing-building-an-http-server-from-scratch-in-c-2da7c0db6cb7">Building an HTTP Server From Scratch in C++</a> <br>
<a href="https://www.benmandrew.com/articles/unix-select-syscall">Using the select System Call in Unix</a> <br>
<a href="https://www.baeldung.com/cs/tcp-protocol-syn-ack">SYN/ACK in the TCP Protocolk</a> <br>
<a href="https://robotecture.com/http-topics/http-headers/">Understanding HTTP Headers</a> <br>
<a href="https://www.garshol.priv.no/download/text/http-tut.html#app">HTTP and CGI explained</a> <br>
<a href="https://www.youtube.com/watch?v=gSQoA4SYhJY">What happens before the Backend gets the Request</a> <br>



[def]: #42webserv
