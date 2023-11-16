/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdeel-o <abdeel-o@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/28 12:45:55 by abdeel-o          #+#    #+#             */
/*   Updated: 2023/11/15 13:16:59 by abdeel-o         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Header guard
#pragma once

#include "Webserv.hpp"

struct RouteConfig {
    std::string 				path;
    std::vector<std::string>	acceptedMethods;
    std::string 				redirection;
    std::string 				rootDirectory;
    bool 						directoryListing;
    std::string 				defaultFile;
    std::string 				cgiExtension;
    bool 						acceptUploads;
    std::string 				uploadDirectory;
};      

class Server
{
	private:
	// proberties
		in_addr_t 						_host;
		uint16_t 						_port;
		std::vector<std::string> 		_serverNames;
		unsigned long 					_clientBodySizeLimit;
		std::vector<RouteConfig> 		_routes;
		std::string 					_root;
		std::string 					_index;
		bool 							_autoindex;
		// struct sockaddr_in 			_server_address;
		std::map<short, std::string>	_error_pages;
        int     						_listen_fd;
		
	public:
	// Constructors
		Server( void );
		~Server( void );
	
	// Getters
	in_addr_t						getHost( void ) const;
	int 							getPort( void ) const;
	std::vector<std::string> 		getServerNames( void ) const;
	int 							getClientBodySizeLimit( void ) const;
	std::vector<RouteConfig> 		getRoutes( void ) const;
	RouteConfig						getRoute( int index ) const;
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
	void	setDefaultErrorPage( std::string );
	void	setClientBodySizeLimit( std::string );
	void	setRoutes( std::vector<RouteConfig> );
	void	setRoute( RouteConfig );
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
