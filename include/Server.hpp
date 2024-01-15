/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdeel-o <abdeel-o@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/28 12:45:55 by abdeel-o          #+#    #+#             */
/*   Updated: 2024/01/15 12:27:46 by abdeel-o         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Header guard
#pragma once

#include "Webserv.hpp"

#define SOCKET int
#define ISVALIDSOCKET(s) ((s) != -1)
#define CLOSESOCKET(s) close(s)

#define REQUEST_BUFFER_SIZE 65536

class Client;

class Location;

class Server
{
	private:
	// proberties
		in_addr_t 						_host;
		std::string 					_host_string;
		uint16_t 						_port;
		std::string 					_port_string;
		std::vector<std::string> 		_serverNames;
		unsigned long 					_clientBodySizeLimit;
		std::string 					_root;
		std::string 					_index;
		bool 							_autoindex;
		struct sockaddr_in 				_server_address; 
		std::map<short, std::string>	_error_pages;
        SOCKET     						_listen_socket;
		std::vector<Location>			_locations;
		std::vector<std::string>		_acceptedMethods;
		
	public:
	// Constructors
		Server( void );
		~Server( void );
	
	// Getters
	in_addr_t						getHost( void ) const;
	int 							getPort( void ) const;
	std::vector<std::string> 		getServerNames( void ) const;
	unsigned long 					getClientBodySizeLimit( void ) const;
	std::vector<Location>			getLocations( void ) const;
	Location						getLocation( int index ) const;
	std::string 					getRoot( void ) const;
	std::string 					getIndex( void ) const;
	bool 							getAutoindex( void ) const;
	int 							getListenFd( void ) const;
	std::map<short, std::string>	getErrorPages( void ) const;
	std::string 					getErrorPage( short number ) const;
	std::string 					getHostString( void ) const;
	std::string 					getPortString( void ) const;
	std::vector<std::string>		getAcceptedMethods( void ) const;
	
	// Setters
	void	setHost( std::string );
	void	setPort( std::string );
	void	setServerNames( std::vector<std::string> );
	void	setClientBodySizeLimit( std::string );
	void	setLocations( std::vector<Location> );
	void	setLocation( Location );
	void	setRoot( std::string );
	void	setIndex( std::string );
	void	setAutoindex( bool );
	void	setErrorPage( std::vector<std::string> );
	void	setListenFd( int );
	void	setAcceptedMethods( std::vector<std::string> );
	
	Location	*getMatchingLocation( std::string uri );
	
	// Operators
	Server &operator=( Server const &rhs );
	Server( Server const &rhs );

	std::string getMimeType( std::string extension );
	
	// Methods
	void init( void );
	void acceptConnection( fd_set &set );
	void handleRequest( int fd, Client& client );
	void sendResponse( int fd, Client& client );
	void send_400( int fd, Client& client );
	void printErrorPages() ;
};
