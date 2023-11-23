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

// Constructors & Destructors
Http::Http( void )
{
}
Http::Http( std::vector<Server> servers ) : _servers(servers)
{
}
Http::~Http( void )
{
}
// Methods
void	Http::initServers( void )
{
	Logger::getInstance().log("Initializing servers...", 2);
	FD_ZERO(&_read_set);
	FD_ZERO(&_write_set);
	for (u_short i = 0; i < _servers.size(); i++)
	{
		_servers[i].init();
		_fd_server_map[_servers[i].getListenFd()] = _servers[i];
		addFDToSet(_servers[i].getListenFd(), &_read_set);
	}
	_max_fd = _servers[_servers.size() - 1].getListenFd();
	/*[Debug]*/ std::cout << "max_fd: " << _max_fd << std::endl;
}
void	Http::run( void )
{
	Logger::getInstance().log("Running servers...", 2);
	while (true)
		; // TODO: to be continued...
}

void 	Http::addFDToSet( int fd, fd_set *set )
{
	if (fd > _max_fd)
		_max_fd = fd;
	FD_SET(fd, set);
}