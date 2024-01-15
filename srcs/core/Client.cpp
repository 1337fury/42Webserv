/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdeel-o <abdeel-o@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/24 11:48:12 by abdeel-o          #+#    #+#             */
/*   Updated: 2024/01/15 17:03:38 by abdeel-o         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client( void ) {
	this->request = Request();
	this->_clientSock = -1;
	this->_clientAddr = sockaddr_in();
	this->_addrLen = 0;
	this->_server = Server();
	this->response = NULL;
	this->reqParser = RequestParser();
}

Client::Client( SOCKET clientSock, sockaddr_in clientAddr, Server& server ) {
	this->request = Request();
	this->_clientSock = clientSock;
	this->_clientAddr = clientAddr;
	this->_addrLen = sizeof(this->_clientAddr);
	this->_server = server;
	this->response = NULL;
	this->reqParser = RequestParser();
}

Client::~Client( void ) {
}

Client::Client( const Client & src ) {
	*this = src;
}

Client & Client::operator=( const Client & rhs ) {
	if ( this != &rhs ) {
		this->_clientSock = rhs._clientSock;
		this->_clientAddr = rhs._clientAddr;
		this->_addrLen = rhs._addrLen;
		this->request = rhs.request;
		this->_server = rhs._server;
		this->response = rhs.response;
		this->reqParser = rhs.reqParser;
	}
	return *this;
}

// Getters
SOCKET			Client::getClientSock( void ) const {
	return this->_clientSock;
}

sockaddr_in		Client::getClientAddr( void ) const {
	return this->_clientAddr;
}

socklen_t		Client::getClientAddrLen( void ) const {
	return this->_addrLen;
}

Request&			Client::getRequest( void ) {
	return this->request;
}

Server			Client::getServer( void ) const {
	return this->_server;
}

// Setters
void			Client::setClientSock( SOCKET clientSock ) {
	this->_clientSock = clientSock;
}

void			Client::setClientAddr( sockaddr_in clientAddr ) {
	this->_clientAddr = clientAddr;
}

void			Client::setClientAddrLen( socklen_t addrLen ) {
	this->_addrLen = addrLen;
}

void			Client::setServer( Server& server ) {
	this->_server = server;
}

