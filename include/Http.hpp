/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Http.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdeel-o <abdeel-o@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/19 12:49:21 by abdeel-o          #+#    #+#             */
/*   Updated: 2023/11/19 12:51:38 by abdeel-o         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Server.hpp"
#include "Webserv.hpp"

class Http
{
	private:
	// Properties
		std::vector<Server> 	_servers;
		fd_set					_read_set;
		fd_set					_write_set;
		int						_max_fd;
		std::map<int, Server>	_fd_server_map;
		std::map<int, Client>	_fd_client_map;
		struct timeval			_timeout;
		fd_set					_read_set_copy;
		fd_set					_write_set_copy;
	// Methods
		
		Http( void );
	public:
	// Constructors & Destructors
		Http( std::vector<Server> servers );
		~Http( void );
	// Mtethods
		void	initServers( void );
		void	run( void );
		void 	addFDToSet( int fd, fd_set *set );
};
