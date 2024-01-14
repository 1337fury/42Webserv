/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdeel-o <abdeel-o@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/28 10:54:37 by abdeel-o          #+#    #+#             */
/*   Updated: 2024/01/14 15:47:21 by abdeel-o         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Webserv.hpp"
#include "Server.hpp"
#include "Location.hpp"
#include "Request.hpp"

#include <map>
#include <vector>

typedef std::map<std::string, std::string>	map_str_str;
typedef std::vector<std::string>			vec_str;
typedef int									fd_t;	

class CGI
{
	public:
		CGI(Location &location, std::string &path);
		CGI(const CGI &src);
		CGI &operator=(const CGI &src);
		~CGI();

		bool		validate( void );
		bool		execute(Request &request);
		bool		isExecuted() const;
		int			wait( void );

		std::string	getErrorMsg() const;
		bool 		setCgiEnvs(Request &request);
		bool		setupFiles( void );
		bool 		create_body_file(Request &request);
		fd_t		getStdout() const;
		fd_t		getStderr() const;
		pid_t		getPid() const;
	
	private:
		const char*		_args[3];
		FILE*		_body_file;
		pid_t 		_cgi_prcs;
		char**		_env;
		std::string	_error_msg;
		std::string	_file_path;
		std::string	_bin_path;
		bool		_is_executed;
		map_str_str _cgi_envs;
		vec_str		_cgi_args;
		fd_t		_cgi_stdout;
		fd_t		_cgi_stderr;
};