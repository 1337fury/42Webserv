/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdeel-o <abdeel-o@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/28 12:52:40 by abdeel-o          #+#    #+#             */
/*   Updated: 2024/01/15 19:14:24 by abdeel-o         ###   ########.fr       */
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
	this->_host_string = "localhost";
	this->_port_string = "";
	this->_acceptedMethods = std::vector<std::string>();
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
		this->_host_string = rhs._host_string;
		this->_port_string = rhs._port_string;
		this->_acceptedMethods = rhs._acceptedMethods;
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
unsigned long	Server::getClientBodySizeLimit( void ) const {
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
	if (this->_error_pages.count(number) == 0)
		return "";
	return this->_error_pages.at(number);
}
int	Server::getListenFd( void ) const {
	return this->_listen_socket;
}
std::string	Server::getHostString( void ) const {
	return this->_host_string;
}
std::string	Server::getPortString( void ) const {
	return this->_port_string;
}
std::vector<std::string>	Server::getAcceptedMethods( void ) const {
	return this->_acceptedMethods;
}

// Setters
void	Server::setHost( std::string host ) {
	struct in_addr addr;
	if (host == "localhost")
		host = "127.0.0.1";
	if (host.length() > 9 || !inet_pton(AF_INET, host.c_str(), &addr))
		throw std::runtime_error("nginy: Invalid host address");
	this->_host_string = host;
	this->_host = inet_addr(host.c_str());
}
void	Server::setPort(std::string port) {
    if (port.length() > 5 || !isNumber(port))
		throw std::runtime_error("nginy: Invalid port number");
	this->_port_string = port;
    this->_port = atoi(port.c_str());
}
void	Server::setServerNames( std::vector<std::string> serverNames ) {
	this->_serverNames = serverNames;
}

void	Server::setClientBodySizeLimit( std::string clientBodySizeLimit ) {
	if (clientBodySizeLimit.length() > 10 || !isNumber(clientBodySizeLimit))
		throw std::runtime_error("nginy: client_max_body_size directive invalid value");
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
		throw std::runtime_error("nginy: Invalid error page path");
	if (checks_type(path) != REG_FILE)
		throw std::runtime_error("nginy: Error page not accessible");
	while (parameters.size() > 0)
	{
		if (parameters.back().length() != 3 || !isNumber(parameters.back()))
			throw std::runtime_error("nginy: Invalid error page status code");
		this->_error_pages[atoi(parameters.back().c_str())] = path;
		parameters.pop_back();
	}
}
void	Server::setListenFd( int listen_fd ) {
	this->_listen_socket = listen_fd;
}
void	Server::setAcceptedMethods( std::vector<std::string> acceptedMethods ) {
	check_methods(acceptedMethods);
	this->_acceptedMethods = acceptedMethods;
}

std::string	Server::getMimeType( std::string extension ) {
	std::map<std::string, std::string> mime_types;
	mime_types["css"] = "text/css";
	mime_types["csv"] = "text/csv";
	mime_types["gif"] = "image/gif";
	mime_types["htm"] = "text/html";
	mime_types["html"] = "text/html";
	mime_types["ico"] = "image/x-icon";
	mime_types["png"] = "image/png";
	mime_types["jpeg"] = "image/jpeg";
	mime_types["jpg"] = "image/jpeg";
	mime_types["js"] = "application/javascript";
	mime_types["json"] = "application/json";
	mime_types["png"] = "image/png";
	mime_types["pdf"] = "application/pdf";
	mime_types["svg"] = "image/svg+xml";
	mime_types["txt"] = "text/plain";
	if (mime_types.count(extension) == 0)
		return "application/octet-stream";
	return mime_types[extension];
}

Location	*Server::getMatchingLocation( std::string uri ) { 
	Location *location = NULL;
	size_t longest_prefix = 0;
	for ( std::vector<Location>::iterator it = this->_locations.begin(); it != this->_locations.end(); it++ )
	{
		if ( uri.find(it->getPath()) == 0 && it->getPath().length() > longest_prefix )
		{
			longest_prefix = it->getPath().length();
			location = &(*it);
		}
	}
	if ( location == NULL && this->getRoot().empty() == false )
	{
		location = new Location();
		location->heapAllocated = true;
		location->setPath("/");
		location->setRootDirectory(_root);
		location->setDefaultFile(_index);
		location->setAutoindex(_autoindex);
	}
	return location;
}

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
		Logger::getInstance().log(COLOR_RED, "nginy: Socket creation failed");
		exit(EXIT_FAILURE);
	}
	int opt = 1;
	if (setsockopt(this->_listen_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1) {
		Logger::getInstance().log(COLOR_RED, "nginy: Socket options failed");
		exit(EXIT_FAILURE);
	}
	memset(&_server_address, 0, sizeof(_server_address));
	_server_address.sin_family = AF_INET; // IPv4
	_server_address.sin_addr.s_addr = this->_host; // INADDR_ANY
	_server_address.sin_port = htons(this->_port); // Port
	if (bind(this->_listen_socket, (struct sockaddr *)&_server_address, sizeof(_server_address)) == -1) {
		Logger::getInstance().log(COLOR_RED, "nginy: Socket binding failed");
		exit(EXIT_FAILURE);
	}
	if (listen(this->_listen_socket, 510) == -1) {
		Logger::getInstance().log(COLOR_RED, "nginy: Socket listening failed");
		exit(EXIT_FAILURE);
	}
	if (set_non_blocking(this->_listen_socket) == -1) {
		Logger::getInstance().log(COLOR_RED, "nginy: Socket non-blocking failed");
		exit(EXIT_FAILURE);
	}
}

void   Server::acceptConnection( fd_set &read_set)
{
	sockaddr_in  		client_addr;
	socklen_t           client_addr_len = sizeof(client_addr);
	int                 client_fd;

	client_fd = accept(_listen_socket, (struct sockaddr *)&client_addr, &client_addr_len);
	if (!ISVALIDSOCKET(client_fd))
	{
		Logger::getInstance().log(COLOR_RED, "nginy: Socket accept failed");
		return ;
	}

	Http::addFDToSet(client_fd, &read_set);
	
	if (set_non_blocking(client_fd) == -1)
	{
		Logger::getInstance().log(COLOR_RED, "nginy: Socket non-blocking failed");
		Http::removeFDFromSet(client_fd, &read_set);
		close(client_fd);
		return ;
	}
	Client client(client_fd, client_addr, *this);
	client.setClientAddrLen(client_addr_len);

	if (Http::fd_client_map.count(client_fd) == 1)
		Http::fd_client_map[client_fd] = client;
	else
		Http::fd_client_map.insert(std::pair<SOCKET, Client>(client_fd, client));
}

void	Server::handleRequest( int fd, Client& client ) {
	int parseResult = PARSE_INCOMPLETE;
	char buffer[REQUEST_BUFFER_SIZE];
	int bytes_received = 0;
	bytes_received = recv(fd, buffer, REQUEST_BUFFER_SIZE, 0);
	if (bytes_received == 0)
	{
		Logger::getInstance().log(COLOR_YELLOW, "Connection closed by peer");
		Http::closeConnection(fd);
		return ;
	}
	else if (bytes_received < 0)
	{
		Logger::getInstance().log(COLOR_RED, "nginy: Socket recv failed");
		Http::closeConnection(fd);
		return ;
	}
	else if (bytes_received > 0)
	{
		parseResult = client.reqParser.parse(client.request, buffer, buffer + bytes_received);
		memset(buffer, 0, REQUEST_BUFFER_SIZE);
	}
	if (parseResult == PARSE_SUCCESS || parseResult == PARSE_ERROR)
	{
		if (parseResult == PARSE_ERROR)
		{
			Logger::getInstance().log(COLOR_RED, "Sending 400 Bad Request");
			send_400(fd, client);
			return ;
		}
		Response *response = new Response(client.request, *this);
		response->create(client);
		client.response = response;
		Http::removeFDFromSet(fd, &Http::read_set);
		Http::addFDToSet(fd, &Http::write_set);
	}
}

void	Server::sendResponse( __unused int fd, __unused Client& client )
{
	std::string response = client.response->getResponseString();
	int bytes_sent = send(client.getClientSock(), response.c_str(), response.length(), 0);
	if (bytes_sent < 0)
		Http::closeConnection(fd);
	else if (bytes_sent == 0 || (size_t)bytes_sent == response.length())
	{
		Http::removeFDFromSet(fd, &Http::write_set);
		Http::addFDToSet(fd, &Http::read_set);
		client.response->reset();
		client.request.reset();
		delete client.response;
		Http::closeConnection(fd);
	}
	else
		client.response->update(bytes_sent);
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