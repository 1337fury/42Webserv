/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdeel-o <abdeel-o@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/24 11:48:07 by abdeel-o          #+#    #+#             */
/*   Updated: 2023/12/02 18:23:27 by abdeel-o         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

// Constructors & Destructors
Response::Response( void )
{
	_status = "200 OK";
	_headers.push_back(Header("Server", "Nginy/1.0"));
	_content = std::vector<char>();
}

Response::~Response( void )
{
}
// Methods
