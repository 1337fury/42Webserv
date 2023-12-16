/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdeel-o <abdeel-o@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/12 19:14:14 by abdeel-o          #+#    #+#             */
/*   Updated: 2023/12/16 17:15:39 by abdeel-o         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

bool	isNumber( std::string str )
{
	for (size_t i = 0; i < str.length(); i++)
		if (!isdigit(str[i]))
			return false;
	return true;
}

std::string status_code( u_short statusCode )
{
	switch (statusCode)
	{
		case 100:
			return "Continue";
		case 101:
			return "Switching Protocols";
		case 102:
			return "Processing";
		case 103:
			return "Early Hints";
		case 200:
			return "OK";
		case 201:
			return "Created";
		case 202:
			return "Accepted";
		case 203:
			return "Non-Authoritative Information";
		case 204:
			return "No Content";
		case 205:
			return "Reset Content";
		case 206:
			return "Partial Content";
		case 207:
			return "Multi-Status";
		case 208:
			return "Already Reported";
		case 226:
			return "IM Used";
		case 300:
			return "Multiple Choices";
		case 301:
			return "Moved Permanently";
		case 302:
			return "Found";
		case 303:
			return "See Other";
		case 304:
			return "Not Modified";
		case 305:
			return "Use Proxy";
		case 306:
			return "Switch Proxy";
		case 307:
			return "Temporary Redirect";
		case 308:
			return "Permanent Redirect";
		case 400:
			return "Bad Request";
		case 401:
			return "Unauthorized";
		case 402:
			return "Payment Required";
		case 403:
			return "Forbidden";
		case 404:
			return "Not Found";
		case 405:
			return "Method Not Allowed";
		case 406:
			return "Not Acceptable";
		case 407:
			return "Proxy Authentication Required";
		case 408:
			return "Request Timeout";
		case 409:
			return "Conflict";
		case 410:
			return "Gone";
		case 411:
			return "Length Required";
		case 412:
			return "Precondition Failed";
		case 413:
			return "Payload Too Large";
		case 414:
			return "URI Too Long";
		case 415:
			return "Unsupported Media Type";
		case 416:
			return "Range Not Satisfiable";
		case 417:
			return "Expectation Failed";
		case 418:
			return "I'm a teapot";
		case 421:
			return "Misdirected Request";
		case 422:
			return "Unprocessable Entity";
		case 423:
			return "Locked";
		case 424:
			return "Failed Dependency";
		case 425:
			return "Too Early";
		case 426:
			return "Upgrade Required";
		case 428:
			return "Precondition Required";
		case 429:
			return "Too Many Requests";
		case 431:
			return "Request Header Fields Too Large";
		case 451:
			return "Unavailable For Legal Reasons";
		case 500:
			return "Internal Server Error";
		case 501:
			return "Not Implemented";
		case 502:
			return "Bad Gateway";
		case 503:
			return "Service Unavailable";
		case 504:
			return "Gateway Timeout";
		case 505:
			return "HTTP Version Not Supported";
		case 506:
			return "Variant Also Negotiates";
		case 507:
			return "Insufficient Storage";
		case 508:
			return "Loop Detected";
		case 510:
			return "Not Extended";
		case 511:
			return "Network Authentication Required";
		default:
			return "Unknown Status Code";
	}
}

int	checks_type(std::string const path)
{
	struct stat		filestat;
	int				ret;

	ret = stat(path.c_str(), &filestat); // stat function used to get file status (type, size, permissions, etc.), returns -1 on error and 0 on success, and fills the struct stat pointed to by filestat with the file status information
	if (ret == -1)
		return (ERROR);
	if (S_ISDIR(filestat.st_mode))
		return (DIRECTORY); // directory
	if (S_ISREG(filestat.st_mode))
		return (REG_FILE); // regular file
	return (OTHER);
}

bool	checks_permissions(std::string const path)
{
	if (access(path.c_str(), F_OK) == -1) 
		return (false); // file does not exist
	if (access(path.c_str(), R_OK) == -1)
		return (false); // file is not readable
	return (true);
}

bool	check_methods(std::vector<std::string>& methods)
{
	std::vector<std::string> validMethods;
	validMethods.push_back("GET");
	validMethods.push_back("POST");
	validMethods.push_back("DELETE");
	for (size_t i = 0; i < methods.size(); i++)
	{
		if (std::find(validMethods.begin(), validMethods.end(), methods[i]) == validMethods.end())
			throw std::invalid_argument("WebServ: [location] Invalid or not supported method `" +  methods[i] + "`");
	}
	return true;
}

int set_non_blocking(int fd)
{
	int flags;

	flags = fcntl(fd, F_GETFL, 0); // get current file status flags
	if (flags == -1)
		return -1;
	return fcntl(fd, F_SETFL, flags | O_NONBLOCK); // set new file status flags
}

// this function is used to normalize the path, example: /var/www/html/../../ -> /var/www, /var/www/html/./ -> /var/www/html, /var/www/html/ -> /var/www/html, because we don't want to serve files outside the root directory, and we don't want to serve the same file twice, example: /var/www/html/index.html and /var/www/html/./index.html are the same file but with different paths so we need to normalize the path to avoid serving the same file twice and to avoid serving files outside the root directory.
std::string normalizePath(std::string& path)
{
	// example: /var/www/html/../../
	std::string normalizedPath;
	std::vector<std::string> tokens;
	std::string token;

	std::istringstream iss(path);
	while (std::getline(iss, token, '/'))
	{
		if (token == "..")
		{
			if (tokens.size() > 0)
				tokens.pop_back();
		}
		else if (token != ".")
			tokens.push_back(token);
	}
	for (size_t i = 0; i < tokens.size(); i++)
	{
		normalizedPath += tokens[i] + "/";
	}
	if (normalizedPath.empty()) // if the path is empty, example: /var/www/html/../../../
		normalizedPath = "/";
	return normalizedPath;
}