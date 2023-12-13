/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdeel-o <abdeel-o@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/28 12:45:55 by abdeel-o          #+#    #+#             */
/*   Updated: 2023/12/13 18:50:56 by abdeel-o         ###   ########.fr       */
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
		
	public:
	// Constructors
		Server( void );
		~Server( void );
	
	// Getters
	in_addr_t						getHost( void ) const;
	int 							getPort( void ) const;
	std::vector<std::string> 		getServerNames( void ) const;
	int 							getClientBodySizeLimit( void ) const;
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
	
	Location	*getMatchingLocation( std::string uri );
	
	// Operators
	Server &operator=( Server const &rhs );
	Server( Server const &rhs );
	
	// Methods
	void init( void ); // init server
	void acceptConnection( fd_set &set ); // accept connection
	void handleRequest( int fd, Client& client ); // handle request
	void handleResponse( int fd, Client& client ); // handle response

	// Error Response
	void send_400( int fd, Client& client );

	// to be deleted
	void printErrorPages() ;
};
