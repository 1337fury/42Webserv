/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdeel-o <abdeel-o@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/28 12:45:55 by abdeel-o          #+#    #+#             */
/*   Updated: 2023/11/16 11:15:38 by abdeel-o         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Header guard
#pragma once

#include "Webserv.hpp"
#include "Location.hpp"

// struct RouteConfig {
//     std::string 				path;
//     std::vector<std::string>	acceptedMethods;
//     std::string 				redirection;
//     std::string 				rootDirectory;
//     bool 						directoryListing;
//     std::string 				defaultFile;
//     std::string 				cgiExtension;
//     bool 						acceptUploads;
//     std::string 				uploadDirectory;
// }; // to be deleted

class Server
{
	private:
	// proberties
		in_addr_t 						_host;
		uint16_t 						_port;
		std::vector<std::string> 		_serverNames;
		unsigned long 					_clientBodySizeLimit;
		// std::vector<RouteConfig> 		_routes; // to be deleted
		std::string 					_root;
		std::string 					_index;
		bool 							_autoindex;
		// struct sockaddr_in 			_server_address; // to be deleted
		std::map<short, std::string>	_error_pages;
        int     						_listen_fd;
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
	// std::vector<RouteConfig> 		getRoutes( void ) const; // to be deleted
	// RouteConfig						getRoute( int index ) const; // to be deleted
	std::string 					getRoot( void ) const;
	std::string 					getIndex( void ) const;
	bool 							getAutoindex( void ) const;
	int 							getListenFd( void ) const;
	std::map<short, std::string>	getErrorPages( void ) const;
	std::string 					getErrorPage( short number ) const;
	
	// Setters
	void	setHost( std::string );
	void	setPort( std::string );
	void	setServerNames( std::vector<std::string> );
	void	setClientBodySizeLimit( std::string );
	void	setLocations( std::vector<Location> );
	void	setLocation( Location );
	// void	setRoutes( std::vector<RouteConfig> ); // to be deleted
	// void	setRoute( RouteConfig ); // to be deleted
	void	setRoot( std::string );
	void	setIndex( std::string );
	void	setAutoindex( bool );
	void	setErrorPage( std::vector<std::string> );
	void	setListenFd( int );
	
	// Operators
	Server &operator=( Server const &rhs );
	Server( Server const &rhs );
	

	// to be deleted
	void printErrorPages() ;
};
