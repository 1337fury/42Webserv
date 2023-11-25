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
	_timeout.tv_sec = 1;
	_timeout.tv_usec = 500000;
	FD_ZERO(&_read_set);
	FD_ZERO(&_write_set);
	_max_fd = 0;
	FD_ZERO(&_read_set_copy);
	FD_ZERO(&_write_set_copy);
}
Http::~Http( void )
{
}
// Methods
void	Http::initServers( void )
{
	Logger::getInstance().log("Initializing servers...", 2);
	for (u_short i = 0; i < _servers.size(); i++)
	{
		_servers[i].init();
		_fd_server_map[_servers[i].getListenFd()] = _servers[i];
		addFDToSet(_servers[i].getListenFd(), &_read_set);
	}
	_max_fd = _servers[_servers.size() - 1].getListenFd();
	/*[Debug]*/ std::cout << "max_fd: " << _max_fd << std::endl;
}

void   Http::acceptConnection( int listen_fd )
{
	struct sockaddr_in  client_addr;
	socklen_t           client_addr_len = sizeof(client_addr);
	int                 client_fd;

	client_fd = accept(listen_fd, (struct sockaddr *)&client_addr, &client_addr_len);
	if (client_fd == -1)
		throw std::runtime_error("accept() failed");
	Logger::getInstance().log("New connection accepted", 2);
	addFDToSet(client_fd, &_read_set);
}

void	Http::run( void )
{
	Logger::getInstance().log("Running servers...", 2);
	while (true)
	{
		_read_set_copy = _read_set;
		_write_set_copy = _write_set;
		if (select(_max_fd + 1, &_read_set_copy, &_write_set_copy, NULL, &_timeout) == -1)
			throw std::runtime_error("select() failed");
		for (int i = 0; i <= _max_fd; i++)
		{
			if (FD_ISSET(i, &_read_set_copy))
			{
				if (i == _servers[0].getListenFd())
					_servers[0].acceptConnection();
				else
					_fd_server_map[i].handleRequest(i);
			}
			if (FD_ISSET(i, &_write_set_copy))
			{
				_fd_server_map[i].handleResponse(i);
			}
		}
	}
}

void 	Http::addFDToSet( int fd, fd_set *set )
{
	if (fd > _max_fd)
		_max_fd = fd;
	FD_SET(fd, set);
}