/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Http.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdeel-o <abdeel-o@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/19 12:52:43 by abdeel-o          #+#    #+#             */
/*   Updated: 2023/11/19 12:53:13 by abdeel-o         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Http.hpp"
#include "Logger.hpp"

std::map<int, Client> Http::fd_client_map = std::map<int, Client>();
fd_set Http::read_set = fd_set(); // creates an empty fd_set
fd_set Http::write_set = fd_set(); // creates an empty fd_set
int Http::max_fd = 0;

// Constructors & Destructors
Http::Http( void )
{
}
Http::Http( std::vector<Server> servers ) : _servers(servers)
{
	_timeout.tv_sec = 1;
	_timeout.tv_usec = 500000;
	FD_ZERO(&Http::read_set);
	FD_ZERO(&Http::write_set);
	FD_ZERO(&read_set_copy);
	FD_ZERO(&_write_set_copy);
	_fd_server_map = std::map<int, Server>();
}
Http::~Http( void )
{
}
// Methods
void	Http::initServers( void )
{
	Logger::getInstance().log(COLOR_BLUE, "Initializing servers...");
	for (u_short i = 0; i < _servers.size(); i++)
	{
		_servers[i].init();
		_fd_server_map[_servers[i].getListenFd()] = _servers[i];
		addFDToSet(_servers[i].getListenFd(), &read_set);
	}
	Http::max_fd = _servers[_servers.size() - 1].getListenFd();
}

/*
	TODO : 
		[√] Handle client connection
	TODO : 
		[I] Handle client request
	TODO : 
		[X] Handle client response
*/

void	Http::run( void )
{
	Logger::getInstance().log(COLOR_BLUE, "Running servers...");
	while (true)
	{
		read_set_copy = read_set;
		_write_set_copy = write_set;
		if (select(Http::max_fd + 1, &read_set_copy, &_write_set_copy, NULL, &_timeout) == -1)
		{
			Logger::getInstance().log(COLOR_RED, "Error: select() failed");
			exit(1);
		}
		for (int i = 0; i <= Http::max_fd; i++)
		{
			if (FD_ISSET(i, &read_set_copy))
			{
				if (_fd_server_map.count(i))
					_fd_server_map[i].acceptConnection(read_set);
				else
					_fd_server_map[GET(i)].handleRequest(i, fd_client_map[i]); // [SIMPLE REQUEST[√] | //! CGI REQUEST[X]
			}
			else if (FD_ISSET(i, &_write_set_copy)) //! In progress
			{
				_fd_server_map[GET(i)].handleResponse(i, fd_client_map[i]);
			}
		}
	}
}

// Static Methods
void 	Http::addFDToSet( int fd, fd_set *set)
{
	if (fd > Http::max_fd)
		Http::max_fd = fd;
	FD_SET(fd, set);
}

void 	Http::removeFDFromSet( int fd, fd_set *set)
{
	FD_CLR(fd, set);
	if (fd == Http::max_fd)
	{
		for (int i = Http::max_fd - 1; i >= 0; i--)
		{
			if (FD_ISSET(i, set))
			{
				Http::max_fd = i;
				break ;
			}
		}
	}
}

void 	Http::closeConnection( int fd )
{
	close(fd);
	removeFDFromSet(fd, &read_set);
	Http::fd_client_map.erase(fd);
}

SOCKET 	Http::getclientServer( int fd )
{
	return (Http::fd_client_map[fd].getServer().getListenFd());
}