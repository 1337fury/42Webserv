/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdeel-o <abdeel-o@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/24 11:47:23 by abdeel-o          #+#    #+#             */
/*   Updated: 2023/12/02 18:24:02 by abdeel-o         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Webserv.hpp"
#include "Server.hpp"

struct Header
{
	std::string		key;
	std::string		value;
};

class Response
{
	private:
	// Properties
		std::string				_status;
		std::vector<Header> 	_headers;
		std::vector<char> 		_content;

	public:
	// Constructors & Destructors
		Response( void );
		~Response( void );
	// Methods
		
};