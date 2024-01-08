/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdeel-o <abdeel-o@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/24 11:47:23 by abdeel-o          #+#    #+#             */
/*   Updated: 2024/01/07 15:45:52 by abdeel-o         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Webserv.hpp"
#include "Server.hpp"
#include "Request.hpp"
#include "Location.hpp"
#include "Cgi.hpp"
#include <sstream>

// Example for an http response:
// HTTP/1.1 200 OK
// Server: Nginy/1.0
// Content-Type: text/html
// Content-Length: 123
// Connection: keep-alive
//
// <html><body><h1>Hello, World!</h1></body></html>
enum reqStatus
{
	LOCATION_NOT_FOUND,
	LOCATIONS_IS_REDIRECTING,
	METHOD_NOT_ALLOWED,
	REQUEST_TO_LARGE,
	PATH_NOT_EXISTING,
	PATH_IS_DIRECTORY,
	PATH_IS_FILE,
	OK
};

class Response
{
	private:
	// Properties
		Server					_server; //? GETTER & SETTER
		u_short					_statusCode; // 200, 404, 500, etc...
		std::string				_status; // 200 OK, 404 Not Found, 500 Internal Server Error, etc...
		std::vector<Header> 	_headers; // Content-Type: text/html, Content-Length: 123, etc...
		std::vector<char> 		_content; // <html><body><h1>Hello, World!</h1></body></html>
		std::string				_body; // <html><body><h1>Hello, World!</h1></body></html>
		Request					_request; // the request that we are responding to
		bool					_keepAlive; // if the connection is keep-alive or not Example: Connection: keep-alive or Connection: close
		std::string				_version; // HTTP/1.1
		
		std::string				_response_string; // the response string that we will send to the client
		
		std::stringstream		_page; // the page that we will send to the client //? GETTER & SETTER
		
		Location				*_location; //! [during testing]
		bool					_error;
		pid_t					_cgi_pid;
		int						_cgi_stdout;
		int						_cgi_stderr;
		bool					_cgi;

	public:
		Response( void );
	// Constructors & Destructors
		Response( Request &request, Server &server );
		Response( Response const &rhs );
		Response &operator=( Response const &rhs );
		~Response( void );
	// Getters
		u_short					getStatusCode( void ) const;
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
		void					setStatusCode( u_short statusCode );
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
		std::string				getCurrentTime();
		void 					init_headers( void );
		void					searchForErrorPage( void );
		reqStatus				analyzeRequest( std::string &path );
		void					create(  Client& client );
		void					reset( void );

	// Response
	void handleRedircetiveLocation( SOCKET clientSock, Redirection red );

	void work_with_directory( SOCKET clientSock );
	void work_with_file( SOCKET clientSock, std::string path );
	void sendResponse( SOCKET clientSock, u_short sCode );
	void initiate_cgi_response( SOCKET clientSock, CGI &cgi );
	void cgiHandler( SOCKET clientSock );
	void handleFileRequest( SOCKET clientSock, std::string path );
};