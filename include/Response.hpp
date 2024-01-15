/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdeel-o <abdeel-o@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/24 11:47:23 by abdeel-o          #+#    #+#             */
/*   Updated: 2024/01/15 12:28:31 by abdeel-o         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Webserv.hpp"
#include "Server.hpp"
#include "Request.hpp"
#include "Location.hpp"
#include "Cgi.hpp"
#include <sstream>

enum reqStatus
{
	LOCATION_NOT_FOUND,
	LOCATIONS_IS_REDIRECTING,
	LOCATION_IS_UPLOADING,
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
		Server					_server;
		u_short					_statusCode;
		std::string				_status;
		std::vector<Header> 	_headers;
		std::vector<char> 		_content;
		std::string				_body;
		Request					_request;
		bool					_keepAlive;
		std::string				_version;
		
		std::string				_response_string;
		
		std::stringstream		_page;
		
		Location				*_location;
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
		void					update( int bytes_sent );

	// Response
		void handleRedircetiveLocation( SOCKET clientSock, Redirection red );

		void work_with_directory( SOCKET clientSock );
		void work_with_file( SOCKET clientSock, std::string path );
		void createResponse( SOCKET clientSock, u_short sCode );
		void initiate_cgi_response( SOCKET clientSock, CGI &cgi );
		void cgiHandler( SOCKET clientSock );
		void handleFileRequest( SOCKET clientSock, std::string path );
		void handle_upload( SOCKET clientSock );

		std::pair<std::string, std::string>	 getData( std::vector<char> content, std::string boundary );
};