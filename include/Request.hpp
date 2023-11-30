/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdeel-o <abdeel-o@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/24 11:46:52 by abdeel-o          #+#    #+#             */
/*   Updated: 2023/11/28 20:26:27 by abdeel-o         ###   ########.fr       */
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
	public:
	// constructors
		Request( void );
		Request( Request const &rhs );
		Request &operator=( Request const &rhs );
		~Request( void );

	// Properties
		std::string				method;
		std::string				uri;
		int 					versionMajor;
		int 					versionMinor;
		std::vector<Header> 	headers;
		std::vector<char> 		content; // we use vector<char> instead of string because string is null terminated and we want to be able to send binary data in the body of the request (like images) and we don't want to stop at the first null character we encounter in the body of the request (if we use string) so we use vector<char> instead, image binary data example: \x89\x50\x4E\x47\x0D\x0A\x1A\x0A so we use vector<char> to be able to store \x00
		bool 					keepAlive;
};