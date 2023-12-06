/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdeel-o <abdeel-o@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/26 16:28:11 by abdeel-o          #+#    #+#             */
/*   Updated: 2023/12/05 12:05:58 by abdeel-o         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

Request::Request( void ) {
	this->method = "";
	this->uri = "";
	this->versionMajor = 1;
	this->versionMinor = 1;
	this->headers = std::vector<Header>();
	this->content = std::vector<char>();
	this->keepAlive = false;
}

Request::Request( Request const &rhs ) {
	*this = rhs;
}

Request &Request::operator=( Request const &rhs ) {
	if ( this != &rhs ) {
		this->method = rhs.method;
		this->uri = rhs.uri;
		this->versionMajor = rhs.versionMajor;
		this->versionMinor = rhs.versionMinor;
		this->headers = rhs.headers;
		this->content = rhs.content;
		this->keepAlive = rhs.keepAlive;
	}
	return *this;
}

Request::~Request( void ) {
}

std::ostream &operator<<( std::ostream &o, Request const &i ) {
	o << "Print Request:" << std::endl;
	o << "\tMethod: " << i.method << std::endl;
	o << "\tUri: " << i.uri << std::endl;
	o << "\tHttpVersion: " << i.versionMajor << "/" << i.versionMinor << std::endl;
	o << "\tHeaders: " << std::endl;
	for (size_t j = 0; j < i.headers.size(); j++)
		o << "\t\t" << i.headers[j].key << ": " << i.headers[j].value << std::endl;
	o << "\tContent: " << std::endl;
	for (size_t j = 0; j < i.content.size(); j++)
		o << i.content[j];
	o << std::endl;
	o << "\tKeepAlive: " << i.keepAlive << std::endl;
	return o;
}