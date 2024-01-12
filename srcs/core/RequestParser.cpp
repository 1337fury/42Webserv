/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestParser.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdeel-o <abdeel-o@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/26 16:51:44 by abdeel-o          #+#    #+#             */
/*   Updated: 2024/01/12 18:56:28 by abdeel-o         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RequestParser.hpp"

RequestParser::RequestParser( void ) : 
_state(REQUEST_METHOD_START),
_contentSize(0),
_chunkSize(0),
_chunked(false)
{
}

RequestParser::~RequestParser( void ) {
}

bool	RequestParser::isChar( char c )
{
	return c >= 0 && c <= 127;
}

// Control characters are those with ASCII codes in the range 0–31 (Information Separator characters) and 127 (DEL).
bool	RequestParser::isCtl( char c )
{
	return (c >= 0 && c <= 31) || (c == 127);
}

bool	RequestParser::isDigit( char c )
{
	return c >= '0' && c <= '9';
}

// The set of characters that are allowed in the HTTP protocol is quite limited.
bool	RequestParser::isTspecial( char c )
{
	switch (c)
	{
		case '(': case ')': case '<': case '>': case '@':
		case ',': case ';': case ':': case '\\': case '"':
		case '/': case '[': case ']': case '?': case '=':
		case '{': case '}': case ' ': case '\t':
			return true;
		default:
			return false;
	}
}

ParseResult	RequestParser::parse( Request &request, const char *begin, const char *end )
{
	const char *current = begin;
	while (current != end)
	{
		switch (_state)
		{
			case REQUEST_METHOD_START:
				if (!isChar(*current) || isCtl(*current) || isTspecial(*current))
					return PARSE_ERROR ;
				else
				{
					_state = REQUEST_METHOD;
					request.method.push_back(*current);
				}
				break;
			case REQUEST_METHOD:
				if (*current == ' ')
					_state = URI_START;
				else if (!isChar(*current) || isCtl(*current) || isTspecial(*current))
					return PARSE_ERROR;
				else
					request.method.push_back(*current);
				break;
			case URI_START:
				if (isCtl(*current) || *current != '/')
					return PARSE_ERROR;
				else
				{
					_state = URI;
					request.uri.push_back(*current);
				}
				break;
			case URI:
				if (*current == ' ')
					_state = HTTP_VERSION_H;
				else if (*current == '?')
					_state = QUERY_STRING_START;
				else if (*current == '#')
					_state = FRAGMENT_START;
				else if (isCtl(*current))
					return PARSE_ERROR;
				else
					request.uri.push_back(*current);
				break;
			case QUERY_STRING_START:
				if (isCtl(*current))
					return PARSE_ERROR;
				else if (*current == '#')
					_state = FRAGMENT_START;
				else
				{
					_state = QUERY_STRING;
					request.query_string.push_back(*current);
				}
				break;
			case QUERY_STRING:
				if (*current == ' ')
					_state = HTTP_VERSION_H;
				else if (*current == '#')
					_state = FRAGMENT_START;
				else if (isCtl(*current))
					return PARSE_ERROR;
				else
					request.query_string.push_back(*current);
				break;
			case FRAGMENT_START:
				if (isCtl(*current))
					return PARSE_ERROR;
				else
				{
					_state = FRAGMENT;
					request.fragment.push_back(*current);
				}
				break;
			case FRAGMENT:
				if (*current == ' ')
					_state = HTTP_VERSION_H;
				else if (isCtl(*current))
					return PARSE_ERROR;
				else
					request.fragment.push_back(*current);
				break;
			case HTTP_VERSION_H:
				if (*current == 'H')
					_state = HTTP_VERSION_HT;
				else
					return PARSE_ERROR;
				break;
			case HTTP_VERSION_HT:
				if (*current == 'T')
					_state = HTTP_VERSION_HTT;
				else
					return PARSE_ERROR;
				break;
			case HTTP_VERSION_HTT:
				if (*current == 'T')
					_state = HTTP_VERSION_HTTP;
				else
					return PARSE_ERROR;
				break;
			case HTTP_VERSION_HTTP:
				if (*current == 'P')
					_state = HTTP_VERSION_SLASH;
				else
					return PARSE_ERROR;
				break;
			case HTTP_VERSION_SLASH:
				if (*current == '/')
					_state = HTTP_VERSION_MAJOR_START;
				else
					return PARSE_ERROR;
				break;
			case HTTP_VERSION_MAJOR_START:
				if (isDigit(*current))
				{
					_state = HTTP_VERSION_MAJOR;
					request.versionMajor = *current - '0';
				}
				else
					return PARSE_ERROR;
				break;
			case HTTP_VERSION_MAJOR:
				if (*current == '.')
					_state = HTTP_VERSION_MINOR_START;
				else if (isDigit(*current))
					request.versionMajor = request.versionMajor * 10 + (*current - '0');
				else
					return PARSE_ERROR;
				break;
			case HTTP_VERSION_MINOR_START:
				if (isDigit(*current))
				{
					request.versionMinor = *current - '0';
					_state = HTTP_VERSION_MINOR; 
				}
				else
					return PARSE_ERROR;
				break;
			case HTTP_VERSION_MINOR:
				if (*current == '\r')
					_state = HTTP_VERSION_CR;
				else if (isDigit(*current))
					request.versionMinor = request.versionMinor * 10 + (*current - '0');
				else
					return PARSE_ERROR;
				break;
			case HTTP_VERSION_CR:
				if (*current == '\n')
					_state = HEADER_LINE_START;
				else
					return PARSE_ERROR;
				break;
			case HEADER_LINE_START:
				if (*current == '\r')
					_state = EXPECTING_NEWLINE_3;
				else if (!request.headers.empty() && (*current == ' ' || *current == '\t'))
					_state = HEADER_LWS;
				else if (!isChar(*current) || isCtl(*current) || isTspecial(*current))
					return PARSE_ERROR;
				else
				{
					request.headers.push_back(Header("", ""));
					request.headers.back().key.push_back(*current);
					_state = HEADER_KEY;
				}
				break;
			case HEADER_LWS:
				if (*current == '\r')
					_state = EXPECTING_NEWLINE_2;
				else if (*current == ' ' || *current == '\t')
					;
				else if (isCtl(*current))
					return PARSE_ERROR;
				else
				{
					request.headers.push_back(Header("", ""));
					request.headers.back().key.push_back(*current);
					_state = HEADER_KEY;
				}
				break;
			case HEADER_KEY:
				if (*current == ':')
					_state = SPACE_BEFORE_HEADER_VALUE;
				else if (!isChar(*current) || isCtl(*current) || isTspecial(*current))
					return PARSE_ERROR;
				else
					request.headers.back().key.push_back(*current);
				break;
			case SPACE_BEFORE_HEADER_VALUE:
				if (*current == ' ')
					_state = HEADER_VALUE;
				else
					return PARSE_ERROR;
				break;
			case HEADER_VALUE:
				if (*current == '\r')
				{
					if (request.method == "POST")
					{
						Header &h = request.headers.back();
						if (h.key == "Content-Length")
						{
							_contentSize = atoi(h.value.c_str());
							request.content.reserve(_contentSize);
						}
						else if (h.key == "Transfer-Encoding" && h.value == "chunked")
							_chunked = true;
					}
					_state = EXPECTING_NEWLINE_2;
				}
				else if (isCtl(*current))
					return PARSE_ERROR;
				else
					request.headers.back().value.push_back(*current);
				break;
			case EXPECTING_NEWLINE_2:
				if (*current == '\n')
					_state = HEADER_LINE_START;
				else
					return PARSE_ERROR;
				break;
			case EXPECTING_NEWLINE_3:
				if (_chunked)
					_state = CHUNKED_BODY_SIZE;
				else if (_contentSize == 0)
				
				{
					if (*current == '\n')
						return PARSE_SUCCESS;
					else
						return PARSE_ERROR;
				}
				else
					_state = POST_BODY;
				break;
			case POST_BODY:
				--_contentSize;
				request.content.push_back(*current);
				if (_contentSize == 0)
					return PARSE_SUCCESS;
				break;
			case CHUNKED_BODY_SIZE:
				if (isalnum(*current))
					_chunkSizeStr.push_back(*current);
				else if (*current == '\r')
					_state = CHUNKED_SIZE_NEWLINE;
				else
					return PARSE_ERROR;
				break;
			case CHUNKED_SIZE_NEWLINE:
				if (*current == '\n')
				{
					_chunkSize = strtol(_chunkSizeStr.c_str(), NULL, 16);
					_chunkSizeStr.clear();
					request.content.reserve(request.content.size() + _chunkSize);
					if (_chunkSize == 0)
						_state = CHUNKED_SIZE_NEWLINE_2;
					else
						_state = CHUNKED_DATA;
				}
				else
					return PARSE_ERROR;
				break;
			case CHUNKED_SIZE_NEWLINE_2:
				if (*current == '\r')
					_state = CHUNKED_SIZE_NEWLINE_3;
				else
					return PARSE_ERROR;
				break;
			case CHUNKED_SIZE_NEWLINE_3:
				if (*current == '\n')
					return PARSE_SUCCESS;
				else
					return PARSE_ERROR;
				break;
			case CHUNKED_DATA:
				request.content.push_back(*current);
				--_chunkSize;
				if (_chunkSize == 0)
					_state = CHUNKED_DATA_NEWLINE;
				break;
			case CHUNKED_DATA_NEWLINE:
				if (*current == '\r')
					_state = CHUNKED_DATA_NEWLINE_2;
				else
					return PARSE_ERROR;
				break;
			case CHUNKED_DATA_NEWLINE_2:
				if (*current == '\n')
					_state = CHUNKED_BODY_SIZE;
				else
					return PARSE_ERROR;
				break;
			default:
				return PARSE_ERROR;
		}
		current++;
	}
	return PARSE_INCOMPLETE;
}
 