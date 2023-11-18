/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdeel-o <abdeel-o@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/28 12:52:40 by abdeel-o          #+#    #+#             */
/*   Updated: 2023/11/16 11:16:41 by abdeel-o         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server( void ) {
	this->_host = 0;
	this->_port = 0;
	this->_serverNames = std::vector<std::string>();
	this->_clientBodySizeLimit = 0;
	// this->_routes = std::vector<RouteConfig>(); // to be deleted
	this->_listen_fd = 0;
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
		// this->_routes = rhs._routes;
		this->_root = rhs._root;
		this->_index = rhs._index;
		this->_autoindex = rhs._autoindex;
		this->_error_pages = rhs._error_pages;
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
// std::vector<RouteConfig>	Server::getRoutes( void ) const {
// 	return this->_routes;
// } // to be deleted
// RouteConfig	Server::getRoute( int index ) const {
// 	return this->_routes[index];
// } // to be deleted
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
	return this->_listen_fd;
}

// Setters
void	Server::setHost( std::string host ) {
	struct in_addr addr;
	if (host == "localhost")
		host = "127.0.0.1";
	if (host.length() > 9 || !inet_pton(AF_INET, host.c_str(), &addr))
		throw std::runtime_error("WebServ: Invalid host address");
	this->_host = inet_addr(host.c_str());
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
// void	Server::setRoutes( std::vector<RouteConfig> routes ) {
// 	this->_routes = routes;
// } // to be deleted
// void	Server::setRoute( RouteConfig route ) {
// 	this->_routes.push_back(route);
// } // to be deleted
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
	this->_listen_fd = listen_fd;
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