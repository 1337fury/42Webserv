/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdeel-o <abdeel-o@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/24 11:46:52 by abdeel-o          #+#    #+#             */
/*   Updated: 2024/01/15 11:46:13 by abdeel-o         ###   ########.fr       */
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
	Header(const std::string& key, const std::string& value)
        : key(key), value(value) {}
};


class Request
{
	public:
	// constructors
		Request( void );
		Request( Request const &rhs );
		Request &operator=( Request const &rhs );
		~Request( void );

	// Properties
		std::string				query_string;
		std::string				fragment;
		std::string				method;
		std::string				uri;
		int 					versionMajor;
		int 					versionMinor;
		std::vector<Header> 	headers;
		std::vector<char> 		content;
		bool 					keepAlive;
		std::string				getHeader( const std::string& key ) const;
		void					reset( void );
};

std::ostream &operator<<( std::ostream &o, Request const &rhs );