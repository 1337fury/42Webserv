/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdeel-o <abdeel-o@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/28 10:56:25 by abdeel-o          #+#    #+#             */
/*   Updated: 2024/01/15 18:24:18 by abdeel-o         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cgi.hpp"
#include "Logger.hpp"

CGI::CGI(Location &location, std::string &path) : _env(NULL)
{
	_file_path = path;
	_bin_path = location.getCgiPath();
	_error_msg = "Something went wrong while executing the CGI script";
	_cgi_prcs = -1;
	_is_executed = false;
	_body_file = NULL;
	_cgi_stdout = -1;
	_cgi_stderr = -1;
	_cgi_envs.clear();
	_cgi_args.clear();
}

CGI::CGI(const CGI &src)
{
	*this = src;
}

CGI &CGI::operator=(const CGI &src)
{
	if (this != &src)
	{
		_file_path = src._file_path;
		_bin_path = src._bin_path;
		_is_executed = src._is_executed;
		_env = src._env;
		_error_msg = src._error_msg;
		_cgi_prcs = src._cgi_prcs;
		_body_file = src._body_file;
		_cgi_stdout = src._cgi_stdout;
		_cgi_stderr = src._cgi_stderr;
		_cgi_envs = src._cgi_envs;
		_cgi_args = src._cgi_args;
	}
	return (*this);
}

CGI::~CGI()
{
	
}

bool	CGI::validate( void )
{
	struct stat		file_stat;
	int				ret;

	memset(&file_stat, 0, sizeof(file_stat));
	ret = stat(_bin_path.c_str(), &file_stat); 
	if (ret == -1)
	{
		_error_msg = "No such file or directory";
		return (false);
	}
	if (S_ISDIR(file_stat.st_mode))
	{
		_error_msg = "Is a directory";
		return (false);
	}
	if (!(file_stat.st_mode & S_IXUSR)) // check if the file is executable	
	{
		_error_msg = "Permission denied";
		return (false);
	}
	return (true);
}

bool	CGI::setCgiEnvs(Request &request)
{
	_cgi_envs["CONTENT_LENGTH"] = ft_itos(request.content.size());
	_cgi_envs["CONTENT_TYPE"] =  request.getHeader("Content-Type");
	_cgi_envs["QUERY_STRING"] = request.query_string;
	_cgi_envs["REQUEST_METHOD"] = request.method;
	_cgi_envs["REQUEST_URI"] = request.uri;
	_cgi_envs["SERVER_NAME"] = request.getHeader("Host");
	_cgi_envs["SERVER_SOFTWARE"] = "nginy/1.0";
	_cgi_envs["REDIRECT_STATUS"] = "200";
	_cgi_envs["HTTP_CONNECTION"] = request.getHeader("Connection");
	_cgi_envs["HTTP_HOST"] = request.getHeader("Host");
	_cgi_envs["HTTP_USER_AGENT"] = request.getHeader("User-Agent");
	_cgi_envs["HTTP_COOKIE"] = request.getHeader("Cookie");
	_cgi_envs["HTTP_ACCEPT"] = request.getHeader("Accept");
	_cgi_envs["HTTP_VERSION"] = "HTTP/1.1";
	_cgi_envs["HTTP_COOKIE"] = request.getHeader("Cookie");
	_cgi_envs["SERVER_PROTOCOL"] = "HTTP/1.1";
	_cgi_envs["SERVER_NAME"] = request.getHeader("Host");
	_cgi_envs["PATH_INFO"] = request.uri;
	_cgi_envs["SERVER_SOFTWARE"] = "nginy/1.0";

	for (map_str_str::iterator it = _cgi_envs.begin(); it != _cgi_envs.end(); it++)
	{
		_cgi_args.push_back(it->first + "=" + it->second);
	}
	_env = new char*[_cgi_args.size() + 1];
	if (_env == NULL)
	{
		_error_msg = "failed to allocate memory for the cgi envirenement variables";
		return (false);
	}
	for (size_t i = 0; i < _cgi_args.size(); i++)
	{
		_env[i] = strdup(_cgi_args[i].c_str());
		if (_env[i] == NULL)
		{
			_error_msg = "failed to allocate memory for the cgi envirenement variables";
			return (false);
		}
	}
	_env[_cgi_args.size()] = NULL;
	if (!this->create_body_file(request))
	{
		_error_msg = "failed to create a temporary file for the request body";
		return (false);
	}
	return (true);
}

bool	CGI::create_body_file(Request &request)
{
	_body_file = tmpfile(); //creates a temporary file
	if (_body_file == NULL)
		return (false);
	for (size_t i = 0; i < request.content.size(); i++)
		fputc(request.content[i], _body_file);
	return (true);
}

bool	CGI::setupFiles( void )
{
	_cgi_stdout = fileno(tmpfile());
	if (_cgi_stdout == -1)
	{
		_error_msg = "failed to create a temporary file for the cgi script output";
		return (false);
	}
	_cgi_stderr = fileno(tmpfile());
	if (_cgi_stderr == -1)
	{
		_error_msg = "failed to create a temporary file for the cgi script error";
		return (false);
	}
	return (true);	
}

bool	CGI::execute(__unused Request &request)
{
	_cgi_prcs = fork();
	if (_cgi_prcs == -1)
	{
		_error_msg = "failed to fork";
		return (false);
	}
	if (_cgi_prcs == 0)
	{
		if (request.method == "POST")
		{
			rewind(_body_file); // sets the file position to the beginning of the file of the given stream
			if (dup2(fileno(_body_file), STDIN_FILENO) == -1)
				_exit(EXIT_FAILURE);
		} 
		if (dup2(_cgi_stdout, STDOUT_FILENO) == -1)
			_exit(EXIT_FAILURE);
		if (dup2(_cgi_stderr, STDERR_FILENO) == -1)
			_exit(EXIT_FAILURE);
		_args[0] = _bin_path.c_str();
		_args[1] = _file_path.c_str();
		_args[2] = NULL;
		if (execve(_args[0], (char* const*)_args, _env) == -1)
			_exit(EXIT_FAILURE);
	}
	return (true);
}

int	CGI::wait( void )
{
	int		status;

	if (waitpid(_cgi_prcs, &status, 0) == -1)
	{
		_error_msg = "failed to wait for the cgi script";
		return (-1);
	}
	if (WIFEXITED(status))
	{
		if (WEXITSTATUS(status) == EXIT_SUCCESS)
			return (0);
		else
		{
			_error_msg = "the cgi script exited with an error";
			return (-1);
		}
	}
	else
	{
		_error_msg = "the cgi script exited with an error";
		return (-1);
	}
}

bool	CGI::isExecuted() const
{
	return (_is_executed);
}

std::string	CGI::getErrorMsg() const
{
	return (_error_msg);
}

fd_t	CGI::getStdout() const
{
	return (_cgi_stdout);
}

fd_t	CGI::getStderr() const
{
	return (_cgi_stderr);
}

pid_t	CGI::getPid() const
{
	return (_cgi_prcs);
}