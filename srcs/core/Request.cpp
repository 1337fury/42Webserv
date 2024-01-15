/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdeel-o <abdeel-o@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/26 16:28:11 by abdeel-o          #+#    #+#             */
/*   Updated: 2024/01/15 11:46:42 by abdeel-o         ###   ########.fr       */
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
	this->query_string = "";
	this->fragment = "";
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
		this->query_string = rhs.query_string;
		this->fragment = rhs.fragment;
	}
	return *this;
}

Request::~Request( void ) {
}

std::string Request::getHeader( const std::string& key ) const {
	for (size_t i = 0; i < this->headers.size(); i++)
		if (this->headers[i].key == key)
			return this->headers[i].value;
	return "";
}

void	Request::reset( void ) {
	this->method = "";
	this->uri = "";
	this->versionMajor = 1;
	this->versionMinor = 1;
	this->headers.clear();
	this->content.clear();
	this->keepAlive = false;
	this->query_string = "";
	this->fragment = "";
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