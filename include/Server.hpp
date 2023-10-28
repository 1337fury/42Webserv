/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdeel-o <abdeel-o@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/28 12:45:55 by abdeel-o          #+#    #+#             */
/*   Updated: 2023/10/28 12:52:16 by abdeel-o         ###   ########.fr       */
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
		int 						_clientBodySizeLimit;
		std::vector<RouteConfig> 	_routes;

	public:
	// Constructors
		Server( void );
		~Server( void );	
};
