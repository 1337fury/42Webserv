/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdeel-o <abdeel-o@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/28 12:45:55 by abdeel-o          #+#    #+#             */
/*   Updated: 2023/10/30 15:43:25 by abdeel-o         ###   ########.fr       */
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
		std::string 				_host;
		int 						_port;
		std::vector<std::string> 	_serverNames;
		std::string 				_defaultErrorPage;
		unsigned long 				_clientBodySizeLimit;
		std::vector<RouteConfig> 	_routes;
		struct sockaddr_in 			_server_address;
        int     					_listen_fd;
		
	public:
	// Constructors
		Server( void );
		~Server( void );
	// Member functions
	void	parseConfig( std::vector<t_token> tokens );
	
	// Getters
	std::string 				getHost( void ) const;
	int 						getPort( void ) const;
	std::vector<std::string> 	getServerNames( void ) const;
	std::string 				getDefaultErrorPage( void ) const;
	int 						getClientBodySizeLimit( void ) const;
	std::vector<RouteConfig> 	getRoutes( void ) const;
	
	// Setters
	void	setHost( std::string host );
	void	setPort( int port );
	void	setServerNames( std::vector<std::string> serverNames );
	void	setDefaultErrorPage( std::string defaultErrorPage );
	void	setClientBodySizeLimit( int clientBodySizeLimit );
	void	setRoutes( std::vector<RouteConfig> routes );
	
	// Operators
	Server &operator=( Server const &rhs );
	Server( Server const &rhs );
	
};
