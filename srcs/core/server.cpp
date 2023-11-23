/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdeel-o <abdeel-o@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/28 12:52:40 by abdeel-o          #+#    #+#             */
/*   Updated: 2023/11/23 15:48:40 by abdeel-o         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Logger.hpp"

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
		Logger::getInstance().log("WebServ: Socket creation failed", 1);
		exit(EXIT_FAILURE);
	}
	int opt = 1;
	if (setsockopt(this->_listen_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1) {
		Logger::getInstance().log("WebServ: Socket options failed", 1);
		exit(EXIT_FAILURE);
	}
	memset(&_server_address, 0, sizeof(_server_address));
	_server_address.sin_family = AF_INET; // AF_INET specifies that we are looking for an IPv4 address
	_server_address.sin_addr.s_addr = this->_host;
	_server_address.sin_port = htons(this->_port); // htons() converts the port number to network byte order
	if (bind(this->_listen_socket, (struct sockaddr *)&_server_address, sizeof(_server_address)) == -1) {
		Logger::getInstance().log("WebServ: Socket binding failed", 1);
		exit(EXIT_FAILURE);
	}
	if (listen(this->_listen_socket, 512) == -1) {
		Logger::getInstance().log("WebServ: Socket listening failed", 1);
		exit(EXIT_FAILURE);
	}
	if (set_non_blocking(this->_listen_socket) == -1) {
		Logger::getInstance().log("WebServ: Socket non-blocking failed", 1);
		exit(EXIT_FAILURE);
	}
}