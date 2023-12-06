/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdeel-o <abdeel-o@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/24 11:48:07 by abdeel-o          #+#    #+#             */
/*   Updated: 2023/12/05 12:38:53 by abdeel-o         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

// Constructors & Destructors
Response::Response( void )
{
	_statusCode = 200;
	_status = "OK";
	// _headers.push_back(Header("Server", "Nginy/1.0"));
	_content = std::vector<char>();
}

Response::Response( Request &request ) : _request(request)
{
	_statusCode = 200;
	_status = "OK";
	// _headers.push_back(Header("Server", "Nginy/1.0"));
	_content = std::vector<char>();
}

Response::~Response( void )
{
}

Response::Response( Response const &rhs )
{
	*this = rhs;
}

Response &Response::operator=( Response const &rhs )
{
	if ( this != &rhs )
	{
		this->_statusCode = rhs._statusCode;
		this->_status = rhs._status;
		this->_headers = rhs._headers;
		this->_content = rhs._content;
		this->_request = rhs._request;
		this->_keepAlive = rhs._keepAlive;
		this->_version = rhs._version;
		this->_body = rhs._body;
		this->_response_string = rhs._response_string;
	}
	return *this;
}

// Getters
unsigned int			Response::getStatusCode( void ) const
{
	return _statusCode;
}

std::string				Response::getStatus( void ) const
{
	return _status;
}

std::vector<Header> 	Response::getHeaders( void ) const
{
	return _headers;
}

Header					Response::getHeader( std::string key ) const
{
	for ( std::vector<Header>::const_iterator it = _headers.begin(); it != _headers.end(); it++ )
	{
		if ( it->key == key )
			return *it;
	}
	return Header();
}

std::vector<char> 		Response::getContent( void ) const
{
	return _content;
}

Request					Response::getRequest( void ) const
{
	return _request;
}

bool					Response::getKeepAlive( void ) const
{
	return _keepAlive;
}

std::string				Response::getVersion( void ) const
{
	return _version;
}

std::string				Response::getBody( void ) const
{
	return _body;
}

std::string				Response::getResponseString( void ) const
{
	return _response_string;
}

// Setters
void					Response::setStatusCode( unsigned int statusCode )
{
	_statusCode = statusCode;
}

void					Response::setStatusMessage( std::string status )
{
	_status = status;
}

void					Response::setHeaders( std::vector<Header> headers )
{
	_headers = headers;
}

void					Response::setHeader( std::string key, std::string value  )
{
	Header header;
	header.key = key;
	header.value = value;
	_headers.push_back(header);
}

void					Response::setContent( std::vector<char> content )
{
	_content = content;
}

void					Response::setRequest( Request request )
{
	_request = request;
}

void					Response::setKeepAlive( bool keepAlive )
{
	_keepAlive = keepAlive;
}

void					Response::setVersion( int major, int minor )
{
	_version = "HTTP/" + std::to_string(major) + "." + std::to_string(minor);
}

void					Response::setBody( void )
{
	_body = std::string(_content.begin(), _content.end());
}

void					Response::setResponseString( void )
{
	_response_string = _version + " " + std::to_string(_statusCode) + " " + _status + "\r\n";
	for ( std::vector<Header>::iterator it = _headers.begin(); it != _headers.end(); it++ )
		_response_string += it->key + ": " + it->value + "\r\n";
	_response_string += "\r\n";
	_response_string += _body;
}

// Methods
void	Response::create( void )
{
	
}