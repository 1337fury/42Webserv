/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdeel-o <abdeel-o@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/28 10:56:25 by abdeel-o          #+#    #+#             */
/*   Updated: 2024/01/01 19:45:50 by abdeel-o         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cgi.hpp"
#include "Logger.hpp"

CGI::CGI(__unused Server &server, Location &location, std::string &path) : _env(NULL)
{
	_file_path = path; // example: /Users/abdeel-o/Desktop/Webserv/cgi-bin/test.py
	_bin_path = location.getCgiPath(); // example: /bin/python3
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

// 1- it checks if the cgi script is a directory or not, if it is a directory it returns false and set the errorMsg to "Is a directory"
// 2- it checks if the cgi script is executable or not, if it is not executable it returns false and set the errorMsg to the error message returned by strerror(errno)
// 3- it calls _setupCgiEnvs to initialize the envirenement variables
bool	CGI::validate( void )
{
	struct stat		file_stat; // struct stat is a struct that contains information about a file
	int				ret;

	memset(&file_stat, 0, sizeof(file_stat)); // memset is used to fill a block of memory with a particular value, here we are filling the file_stat struct with 0
	ret = stat(_bin_path.c_str(), &file_stat); // using stat to get information about the cgi script, like if it is a directory or not, if it is executable or not, etc...
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
	if (!(file_stat.st_mode & S_IXUSR))
	{
		_error_msg = "Permission denied";
		return (false);
	}
	return (true);
}
// set the cgi envirenement variables is for example the request method, the request uri, the request body, etc... is used by the cgi script to process the request, without these variables the cgi script won't be able to process the request, because it won't know what to do with the request, example of cgi envirenement variables: REQUEST_METHOD, REQUEST_URI, QUERY_STRING, CONTENT_LENGTH, CONTENT_TYPE, etc..

bool	CGI::setCgiEnvs(Request &request)
{
	_cgi_envs["CONTENT_LENGTH"] = ft_itos(request.content.size());
	_cgi_envs["CONTENT_TYPE"] = request.getHeader("Content-Type");
	_cgi_envs["QUERY_STRING"] = request.uri;
	_cgi_envs["REQUEST_METHOD"] = request.method;
	_cgi_envs["REQUEST_URI"] = request.uri;
	_cgi_envs["SERVER_NAME"] = request.getHeader("Host");
	_cgi_envs["SERVER_SOFTWARE"] = "nginy/1.0";
	_cgi_envs["REDIRECT_STATUS"] = "200";
	_cgi_envs["HTTP_CONNECTION"] = request.getHeader("Connection");
	_cgi_envs["HTTP_HOST"] = request.getHeader("Host");
	_cgi_envs["HTTP_USER_AGENT"] = request.getHeader("User-Agent");
	_cgi_envs["HTTP_COOKIE"] = request.getHeader("Cookie");
	// init _cgi_envs_str
	for (map_str_str::iterator it = _cgi_envs.begin(); it != _cgi_envs.end(); it++)
	{
		_cgi_args.push_back(it->first + "=" + it->second);
	}
	_env = new char*[_cgi_args.size()];
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
	_body_file = tmpfile();
	if (_body_file == NULL)
		return (false);
	for (size_t i = 0; i < request.content.size(); i++)
		fputc(request.content[i], _body_file);
	return (true);
}

// setupFiles is a function that creates a temporary file for the cgi script output and error, to be able to send them to the client later...
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
		// for POST requests we need to send the body to the cgi script, so we need to redirect the stdin to the body file, we send the body to the cgi script because the cgi script will read the body from the stdin to get the form data and the query string data and the cookie data and the uploaded files data ...,
		if (request.method == "POST")
		{
			rewind(_body_file);
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
	wait(NULL);
	return (true);
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