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
#include "Client.hpp"

#define GET(fd) Http::getclientServer(fd)

class Http
{
	public:
		static std::map<int, Client>	fd_client_map;
		static fd_set					read_set;
		static fd_set					write_set;
		static int						max_fd;
	private:
	// Properties
		std::vector<Server> 	_servers;
		std::map<int, Server>	_fd_server_map;
		struct timeval			_timeout;
		fd_set					read_set_copy;
		fd_set					_write_set_copy;
	// Methods
		
		Http( void );
	public:
	// Constructors & Destructors
		Http( std::vector<Server> servers );
		~Http( void );
	// Mtethods
		void			initServers( void );
		void			run( void );
	// Static Methods
		static void 	addFDToSet( int fd, fd_set *set );
		static void 	removeFDFromSet( int fd, fd_set *set );
		static void 	closeConnection( int fd );
		static SOCKET 	getclientServer( int fd );
};
