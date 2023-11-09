/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdeel-o <abdeel-o@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/28 12:52:40 by abdeel-o          #+#    #+#             */
/*   Updated: 2023/10/31 18:47:05 by abdeel-o         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server( void ) {
}

Server::~Server( void ) {
}

// Getters
std::string	Server::getHost( void ) const {
	return this->_host;
}
int	Server::getPort( void ) const {
	return this->_port;
}
std::vector<std::string>	Server::getServerNames( void ) const {
	return this->_serverNames;
}
std::string	Server::getDefaultErrorPage( void ) const {
	return this->_defaultErrorPage;
}
int	Server::getClientBodySizeLimit( void ) const {
	return this->_clientBodySizeLimit;
}
std::vector<RouteConfig>	Server::getRoutes( void ) const {
	return this->_routes;
}
RouteConfig	Server::getRoute( int index ) const {
	return this->_routes[index];
}

// Setters
void	Server::setHost( std::string host ) {
	this->_host = host;
}
void	Server::setPort( int port ) {
	this->_port = port;
}
void	Server::setServerNames( std::vector<std::string> serverNames ) {
	this->_serverNames = serverNames;
}
void	Server::setDefaultErrorPage( std::string defaultErrorPage ) {
	this->_defaultErrorPage = defaultErrorPage;
}
void	Server::setClientBodySizeLimit( int clientBodySizeLimit ) {
	this->_clientBodySizeLimit = clientBodySizeLimit;
}
void	Server::setRoutes( std::vector<RouteConfig> routes ) {
	this->_routes = routes;
}
void	Server::setRoute( RouteConfig route ) {
	this->_routes.push_back(route);
}
