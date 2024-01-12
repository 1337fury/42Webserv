/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestParser.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdeel-o <abdeel-o@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/26 16:52:17 by abdeel-o          #+#    #+#             */
/*   Updated: 2024/01/12 18:46:15 by abdeel-o         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Webserv.hpp"
#include "Request.hpp"

enum ParseResult
{
	PARSE_SUCCESS,
	PARSE_ERROR,
	PARSE_INCOMPLETE
};

enum RequestState
{
	REQUEST_METHOD_START,
	REQUEST_METHOD,
	URI_START, // 2
	URI,
	QUERY_STRING_START,
	QUERY_STRING,
	FRAGMENT_START,
	FRAGMENT,
	HTTP_VERSION_H,
	HTTP_VERSION_HT,
	HTTP_VERSION_HTT,
	HTTP_VERSION_HTTP,
	HTTP_VERSION_SLASH,
	HTTP_VERSION_MAJOR_START,
	HTTP_VERSION_MAJOR, // 10
	HTTP_VERSION_MINOR_START,
	HTTP_VERSION_MINOR,
	HTTP_VERSION_CR,

	HEADER_LINE_START,
	HEADER_LWS,
	HEADER_KEY,
	SPACE_BEFORE_HEADER_VALUE,
	HEADER_VALUE, // 18
	EXPECTING_NEWLINE_2, // 19
	EXPECTING_NEWLINE_3, // 20

	POST_BODY, // 21
	CHUNKED_BODY_SIZE,
	CHUNKED_SIZE_NEWLINE,
	CHUNKED_SIZE_NEWLINE_2,
	CHUNKED_SIZE_NEWLINE_3,
	
	CHUNKED_DATA_NEWLINE,
	CHUNKED_DATA_NEWLINE_2,
	CHUNKED_DATA
};

class RequestParser
{
	private:
		__unused RequestState	_state;
		__unused size_t 		_contentSize;
    	__unused std::string	_chunkSizeStr;
    	__unused size_t			_chunkSize;
    	__unused bool 			_chunked;
		
	public:
		RequestParser( void );
		~RequestParser( void );
	// Methods
		bool	isChar( char c );
		bool	isCtl( char c );
		bool	isTspecial( char c );
		bool	isDigit( char c );
		ParseResult	parse( Request &request, const char *begin, const char *end );
};
