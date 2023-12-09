/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdeel-o <abdeel-o@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/24 11:43:53 by abdeel-o          #+#    #+#             */
/*   Updated: 2023/12/08 09:47:24 by abdeel-o         ###   ########.fr       */
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
		RequestParser	reqParser;//! [fixed]
		Request			request; //! [NOT VERIFIED]
	private:
		SOCKET			_clientSock;
		sockaddr_in		_clientAddr;
		socklen_t		_addrLen;
		Server			_server; //! [NOT VERIFIED]
		Response		_response; //! [NOT VERIFIED]
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
		Request&		getRequest( void ); //! [NOT VERIFIED]
		Server			getServer( void ) const; //! [NOT VERIFIED]
		Response		getResponse( void ) const; //! [NOT VERIFIED]
		RequestParser	getRequestParser( void ) const; //! [PROBLEM..........]
	// Setters
		void			setClientSock( SOCKET );
		void			setClientAddr( sockaddr_in );
		void			setClientAddrLen( socklen_t );
		void			setRequest( Request& ); //! [NOT VERIFIED]
		void			setServer( Server& ); //! [NOT VERIFIED]
		void			setResponse( Response& ); //! [NOT VERIFIED]
};