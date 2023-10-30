/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdeel-o <abdeel-o@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/28 12:52:40 by abdeel-o          #+#    #+#             */
/*   Updated: 2023/10/30 15:53:12 by abdeel-o         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server( void ) {
}

Server::~Server( void ) {
}

void	Server::parseConfig( std::vector<t_token> tokens ) {
	std::vector<t_token>::iterator it = tokens.begin();
	for (; it != tokens.end(); it++)
	{
		if (it->type == "host")
			this->setHost(it->literal);
	}
}

// Setters
void	Server::setHost( std::string host ) {
	this->host = host;
}
void	Server::setPort( int port ) {
	this->port = port;
}
void	Server::setServerNames( std::vector<std::string> serverNames ) {
	this->serverNames = serverNames;
}
void	Server::setDefaultErrorPage( std::string defaultErrorPage ) {
	this->defaultErrorPage = defaultErrorPage;
}
void	Server::setClientBodySizeLimit( int clientBodySizeLimit ) {
	this->clientBodySizeLimit = clientBodySizeLimit;
}
void	Server::setRoutes( std::vector<RouteConfig> routes ) {
	this->routes = routes;
}
void	Server::setRoute( RouteConfig route ) {
	this->routes.push_back(route);
}
