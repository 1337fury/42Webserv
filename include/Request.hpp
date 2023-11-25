/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdeel-o <abdeel-o@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/24 11:46:52 by abdeel-o          #+#    #+#             */
/*   Updated: 2023/11/24 12:19:09 by abdeel-o         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Webserv.hpp"
#include "Server.hpp"

enum Method
{
	GET,
	HEAD,
	POST,
	PUT,
	DELETE
};

enum Version
{
	HTTP_1_0,
	HTTP_1_1
};

enum Header
{
	Accept_Charsets,
	Accept_Language,
	Allow,
	Authorization,
	Content_Language,
	Content_Length,
	Content_Location,
	Content_Type,
	Date,
	Host,
	Last_Modified,
	Location,
	Referer,
	Retry_After,
	Server,
	Transfer_Encoding,
	User_Agent,
	WWW_Authenticate
};


class Request
{
	private:
	// Properties
		std::string		_method;
		std::string		_uri;
		std::string		_version;
		std::string		_body;
		std::map<Header, std::string>	_headers;
		std::string		_raw_request;
	// Methods
		

	public:
	// Constructors & Destructors
		Request( void );
		~Request( void );
};