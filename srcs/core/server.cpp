/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdeel-o <abdeel-o@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/28 12:52:40 by abdeel-o          #+#    #+#             */
/*   Updated: 2023/12/05 12:51:46 by abdeel-o         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Logger.hpp"
#include "Http.hpp"
#include "Client.hpp"
#include "Request.hpp"
#include "RequestParser.hpp"

Server::Server( void ) {
	this->_host = 0;
	this->_port = 0;
	this->_serverNames = std::vector<std::string>();
	this->_clientBodySizeLimit = 0;
	this->_listen_socket = 0;
	this->_root = "";
	this->_index = "";
	this->_autoindex = false;
	this->_error_pages = std::map<short, std::string>();
	this->_locations = std::vector<Location>();
}

Server::Server( Server const &rhs ) {
	*this = rhs;
}

Server &Server::operator=( Server const &rhs ) {
	if ( this != &rhs ) {
		this->_host = rhs._host;
		this->_port = rhs._port;
		this->_serverNames = rhs._serverNames;
		this->_clientBodySizeLimit = rhs._clientBodySizeLimit;
		this->_listen_socket = rhs._listen_socket;
		this->_root = rhs._root;
		this->_index = rhs._index;
		this->_autoindex = rhs._autoindex;
		this->_error_pages = rhs._error_pages;
		this->_locations = rhs._locations;
	}
	return *this;
}

Server::~Server( void ) {
}

// Getters
in_addr_t	Server::getHost( void ) const {
	return this->_host;
}
int	Server::getPort( void ) const {
	return this->_port;
}
std::vector<std::string>	Server::getServerNames( void ) const {
	return this->_serverNames;
}
int	Server::getClientBodySizeLimit( void ) const {
	return this->_clientBodySizeLimit;
}
std::vector<Location>	Server::getLocations( void ) const {
	return this->_locations;
}
Location	Server::getLocation( int index ) const {
	return this->_locations[index];
}
std::string	Server::getRoot( void ) const {
	return this->_root;
}
std::string	Server::getIndex( void ) const {
	return this->_index;
}
bool	Server::getAutoindex( void ) const {
	return this->_autoindex;
}
std::map<short, std::string>	Server::getErrorPages( void ) const {
	return this->_error_pages;
}
std::string	Server::getErrorPage( short number ) const {
	return this->_error_pages.at(number);
}
int	Server::getListenFd( void ) const {
	return this->_listen_socket;
}

// Setters
void	Server::setHost( std::string host ) {
	struct in_addr addr;
	if (host == "localhost")
		host = "127.0.0.1";
	if (host.length() > 9 || !inet_pton(AF_INET, host.c_str(), &addr)) // inet_pton() converts the IP address from text to binary form
		throw std::runtime_error("WebServ: Invalid host address");
	this->_host = inet_addr(host.c_str()); // Convert the IP address to network byte order
}
void	Server::setPort(std::string port) {
    if (port.length() > 5 || !isNumber(port))
		throw std::runtime_error("WebServ: Invalid port number");
    this->_port = atoi(port.c_str());
}
void	Server::setServerNames( std::vector<std::string> serverNames ) {
	this->_serverNames = serverNames;
}

void	Server::setClientBodySizeLimit( std::string clientBodySizeLimit ) {
	if (clientBodySizeLimit.length() > 10 || !isNumber(clientBodySizeLimit))
		throw std::runtime_error("WebServ: client_max_body_size directive invalid value");
	this->_clientBodySizeLimit = atoi(clientBodySizeLimit.c_str());
}
void	Server::setLocations( std::vector<Location> locations ) {
	this->_locations = locations;
}
void	Server::setLocation( Location location ) {
	this->_locations.push_back(location);
}
void	Server::setRoot( std::string root ) {
	this->_root = root;
}
void	Server::setIndex( std::string index ) {
	this->_index = index;
}
void	Server::setAutoindex( bool autoindex ) {
	this->_autoindex = autoindex;
}
void	Server::setErrorPage( std::vector<std::string> parameters ) {
	std::string path = parameters.back();
	parameters.pop_back();
	if (checks_type(path) != REG_FILE)
		throw std::runtime_error("WebServ: Invalid error page path");
	if (checks_type(path) != REG_FILE)
		throw std::runtime_error("WebServ: Error page not accessible");
	while (parameters.size() > 0)
	{
		if (parameters.back().length() != 3 || !isNumber(parameters.back()))
			throw std::runtime_error("WebServ: Invalid error page status code");
		this->_error_pages[atoi(parameters.back().c_str())] = path;
		parameters.pop_back();
	}
}
void	Server::setListenFd( int listen_fd ) {
	this->_listen_socket = listen_fd;
}

// to be deleted
void Server::printErrorPages()
{
	std::map<short, std::string>::iterator it = this->_error_pages.begin();
	while (it != this->_error_pages.end())
	{
		std::cout << it->first << " " << it->second << std::endl;
		it++;
	}
}

// Methods
void	Server::init( void ) {
	this->_listen_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (!ISVALIDSOCKET(this->_listen_socket)) {
		Logger::getInstance().log(COLOR_RED, "WebServ: Socket creation failed");
		exit(EXIT_FAILURE);
	}
	int opt = 1;
	if (setsockopt(this->_listen_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1) {
		Logger::getInstance().log(COLOR_RED, "WebServ: Socket options failed");
		exit(EXIT_FAILURE);
	}
	memset(&_server_address, 0, sizeof(_server_address));
	_server_address.sin_family = AF_INET; // AF_INET specifies that we are looking for an IPv4 address
	_server_address.sin_addr.s_addr = this->_host;
	_server_address.sin_port = htons(this->_port); // htons() converts the port number to network byte order
	if (bind(this->_listen_socket, (struct sockaddr *)&_server_address, sizeof(_server_address)) == -1) {
		Logger::getInstance().log(COLOR_RED, "WebServ: Socket binding failed");
		exit(EXIT_FAILURE);
	}
	if (listen(this->_listen_socket, 512) == -1) {
		Logger::getInstance().log(COLOR_RED, "WebServ: Socket listening failed");
		exit(EXIT_FAILURE);
	}
	// if (set_non_blocking(this->_listen_socket) == -1) {
	// 	Logger::getInstance().log(COLOR_RED, "WebServ: Socket non-blocking failed");
	// 	exit(EXIT_FAILURE);
	// } //? what Happens if we don't the server socket to non-blocking? : we will have to wait for the client to send data before we can send data to it, and we will have to wait for the client to send data before we can accept another client connection

}

void   Server::acceptConnection( fd_set &read_set)
{
	sockaddr_in  client_addr; // create a structure to hold the address of the client that will connect using the socket
	socklen_t           client_addr_len = sizeof(client_addr); // get the size of the structure
	int                 client_fd; // create a file descriptor for the client socket

	client_fd = accept(_listen_socket, (struct sockaddr *)&client_addr, &client_addr_len);
	if (!ISVALIDSOCKET(client_fd))
	{
		Logger::getInstance().log(COLOR_RED, "WebServ: Socket accept failed");
		exit(EXIT_FAILURE);
	}
	Logger::getInstance().log(COLOR_GREEN, "A connection to `%s` has been established from %s", _serverNames[0].c_str() ,inet_ntoa(client_addr.sin_addr));

	Http::addFDToSet(client_fd, &read_set); //? Can be better
	
	// if (set_non_blocking(client_fd) == -1)
	// {
	// 	Logger::getInstance().log(COLOR_RED, "WebServ: Socket non-blocking failed");
	// 	exit(EXIT_FAILURE);
	// } //? what Happens if we don't set the client socket to non-blocking? : we will have to wait for the client to send data before we can send data to it

	Client client(client_fd, client_addr, *this);
	client.setClientAddrLen(client_addr_len);

	if (Http::fd_client_map.count(client_fd) == 1)
		Http::fd_client_map[client_fd] = client; // update client info because it already exists in the map (client disconnected and reconnected)
		// return ;
	else
		Http::fd_client_map.insert(std::pair<SOCKET, Client>(client_fd, client));
}

void	Server::handleRequest( int fd, Client& client ) {
	Request request; //! Create a request object
	RequestParser requestParser; //! Create a request parser object
	int parseResult = PARSE_INCOMPLETE;
	char buffer[REQUEST_BUFFER_SIZE];
	int bytes_received = 0;
	bytes_received = recv(fd, buffer, REQUEST_BUFFER_SIZE, 0); //* the difference between recv() and read() is that recv() is more flexible and can be used with flags like MSG_DONTWAIT and MSG_WAITALL to make it non-blocking and blocking respectively while read() is always blocking and can't be used with flags (it's a system call)
	if (bytes_received == 0)
	{
		Logger::getInstance().log(COLOR_YELLOW, "Connection closed by peer");
		Http::closeConnection(fd);
	}
	else if (bytes_received == -1)
	{
		Logger::getInstance().log(COLOR_RED, "WebServ: Socket recv failed");
		exit(EXIT_FAILURE);
	}
	else if (bytes_received > 0)
	{
		//? parse request [...]
		parseResult = requestParser.parse(request, buffer, buffer + bytes_received);
		memset(buffer, 0, REQUEST_BUFFER_SIZE);
	}
	if (parseResult == PARSE_SUCCESS || parseResult == PARSE_ERROR)
	{
		client.setRequest(request);
		if (parseResult == PARSE_ERROR)
		{
			Logger::getInstance().log(COLOR_RED, "Sending 400 Bad Request");
			send_400(fd, client);
			return ;
		}
		// TODO: Check if request for CGI process later
		Http::removeFDFromSet(fd, &Http::read_set); // remove fd from read_set because we don't need to read from it anymore
		Http::addFDToSet(fd, &Http::write_set);
	}
	//send response
}

/*
	TODO: Handle request 
	- if request is empty close connection because client closed connection without sending request,it send an ACK to the server and the server recv() returns 0
	- if request is not empty parse it
	- if request is not valid send error response
	- if request is valid send response
	- if request is valid and connection is keep-alive wait for another request
	- if request is valid and connection is not keep-alive close connection
*/

/*
The following steps list a simplified overview of what happens when a client requests a CGI process.

- The client (a web browser) sends a request to the server for a document. If it can, the server responds to the request directly by sending the document.
- If the server determines the request isn't for a document it can simply deliver, the server creates a CGI process.
- The CGI process turns the request information into environment variables. Next, it establishes a current working directory for the child process. Finally, it establishes pipes (data pathways) between the server and an external CGI program.
- After the external CGI program processes the request, it uses the data pathway to send a response back to the server, which in turn, sends the response back to the client.
*/

void	Server::handleResponse( __unused int fd, __unused Client& client )
{
	Request Req;
	Response response(Req); //! Create a response object
	
	
}

void	Server::send_400( int fd, Client& client )
{
	Response response;
	response.setStatusCode(400);
	response.setStatusMessage("Bad Request");
	response.setVersion(client.getRequest().versionMajor, client.getRequest().versionMinor);
	response.setHeader("Content-Type", "text/html");
	response.setHeader("Content-Length", "0");
	response.setHeader("Connection", "close");
	response.setBody();
	response.setResponseString();
	std::string response_str = response.getResponseString();
	send(fd, response_str.c_str(), response_str.length(), 0);
	Http::closeConnection(fd);
}