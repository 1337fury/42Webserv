/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdeel-o <abdeel-o@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/24 11:43:53 by abdeel-o          #+#    #+#             */
/*   Updated: 2024/01/12 18:43:00 by abdeel-o         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Webserv.hpp"
#include "Server.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "RequestParser.hpp"

class Client
{
	public:
		RequestParser	reqParser;
		Request			request;
		
	private:
		SOCKET			_clientSock;
		sockaddr_in		_clientAddr;
		socklen_t		_addrLen;
		Server			_server;
		Response		_response;
	public:
		Client( void );
	// Constructors & Destructors
		Client( SOCKET, sockaddr_in, Server& );
		~Client( void );
		Client( const Client & src );
		Client & operator=( const Client & rhs );
	// Getters
		SOCKET			getClientSock( void ) const;
		sockaddr_in		getClientAddr( void ) const;
		socklen_t		getClientAddrLen( void ) const;
		Request&		getRequest( void );
		Server			getServer( void ) const;
		Response		getResponse( void ) const;
		RequestParser	getRequestParser( void ) const;
	// Setters
		void			setClientSock( SOCKET );
		void			setClientAddr( sockaddr_in );
		void			setClientAddrLen( socklen_t );
		void			setRequest( Request& );
		void			setServer( Server& );
		void			setResponse( Response& );
};