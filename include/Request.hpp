/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdeel-o <abdeel-o@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/24 11:46:52 by abdeel-o          #+#    #+#             */
/*   Updated: 2023/11/26 16:50:41 by abdeel-o         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Webserv.hpp"
#include "Server.hpp"

enum Method
{
	GET,
	POST,
	DELETE
};

struct Header
{
	std::string		key;
	std::string		value;
};


class Request
{
	private:
	// Properties
		// std::string				_method;
		// std::string				_uri;
		// int 					_versionMajor;
		// int 					_versionMinor;
		// std::vector<Header> 	_headers;
		// std::vector<char> 		_content;
		// bool 					_keepAlive;
	public:
	// constructors
		Request( void );
		Request( Request const &rhs );
		Request &operator=( Request const &rhs );
		~Request( void );
	
};