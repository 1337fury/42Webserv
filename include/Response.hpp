/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdeel-o <abdeel-o@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/24 11:47:23 by abdeel-o          #+#    #+#             */
/*   Updated: 2023/12/05 11:56:25 by abdeel-o         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Webserv.hpp"
#include "Server.hpp"
#include "Request.hpp"

class Response
{
	private:
	// Properties
		unsigned int			_statusCode; // 200, 404, 500, etc...
		std::string				_status; // 200 OK, 404 Not Found, 500 Internal Server Error, etc...
		std::vector<Header> 	_headers; // Content-Type: text/html, Content-Length: 123, etc...
		std::vector<char> 		_content; // <html><body><h1>Hello, World!</h1></body></html>
		std::string				_body; // <html><body><h1>Hello, World!</h1></body></html>
		Request					_request; // the request that we are responding to
		bool					_keepAlive; // if the connection is keep-alive or not Example: Connection: keep-alive or Connection: close
		std::string				_version; // HTTP/1.1
		
		std::string				_response_string; // the response string that we will send to the client

	public:
		Response( void );
	// Constructors & Destructors
		Response( Request &request );
		Response( Response const &rhs );
		Response &operator=( Response const &rhs );
		~Response( void );
	// Getters
		unsigned int			getStatusCode( void ) const;
		std::string				getStatus( void ) const;
		std::vector<Header> 	getHeaders( void ) const;
		Header					getHeader( std::string key ) const;
		std::vector<char> 		getContent( void ) const;
		Request					getRequest( void ) const;
		bool					getKeepAlive( void ) const;
		std::string				getVersion( void ) const;
		std::string				getBody( void ) const;
		std::string				getResponseString( void ) const;
	
	// Setters
		void					setStatusCode( unsigned int statusCode );
		void					setStatusMessage( std::string status );
		void					setHeaders( std::vector<Header> headers );
		void					setHeader( std::string key, std::string value  );
		void					setContent( std::vector<char> content );
		void					setRequest( Request request );
		void					setKeepAlive( bool keepAlive );
		void					setVersion( int major, int minor );
		void					setBody( void );
		void					setResponseString( void );
	// Methods
		void	create( void );
};