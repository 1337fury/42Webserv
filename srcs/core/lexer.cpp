/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Lexer.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdeel-o <abdeel-o@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/12 12:15:42 by abdeel-o          #+#    #+#             */
/*   Updated: 2024/01/12 18:53:41 by abdeel-o         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Lexer.hpp"

Lexer::Lexer( std::string const &content ) : 
_content(content),
_line(0),
_position(0),
_column(0)
{
}

void	Lexer::_read( void )
{
	char ch = this->_content[this->_position++];
	if (ch == '\n')
	{
		this->_line++;
		this->_column = 1;
	}
	else
		this->_column++;
}

char Lexer::_peek( void )
{
    return this->_content[this->_position];
}

void Lexer::_skipWhitespace( void )
{
	while (this->_peek() == ' ' || this->_peek() == '\t')
		this->_read();
}

t_token	Lexer::_newToken( std::string type, std::string literal )
{
	t_token	token;
	token.type = type;
	token.literal = literal;
	token.line = this->_line;
	token.column = this->_column;
	return token;
}

t_token	Lexer::_scanComment( void )
{
	this->_read();
	std::string comment = "";
	while (this->_peek() != '\n' && this->_peek() != '\0')
	{
		comment += this->_peek();
		this->_read();
	}
	return this->_newToken("Comment", comment);
}

t_token	Lexer::_scanKeyword( void )
{
	std::string	keyword = "";
	while (
			this->_peek() != ' ' \
			&& this->_peek() != '\t' \
			&& this->_peek() != ';' \
			&& this->_peek() != '{' \
			&& this->_peek() != '}' \
			&& this->_peek() != '\0'
		  )
	{
		keyword += this->_peek();
		this->_read();
	}
	return this->_newToken("Keyword", keyword);
}

t_token	Lexer::_scanQuotedString(char delimiter)
{
	this->_read();
	std::string str = "";
	while (this->_peek() != delimiter && this->_peek() != '\0')
	{
		if (this->_peek() == '\\')
			this->_read();
		str += this->_peek();
		this->_read();
	}
	this->_read();
	return this->_newToken("QuotedString", str);
}

t_token	Lexer::_getNextToken( void )
{
	char	c = this->_peek();
	switch (c)
	{
		case '\n':
			this->_read();
			return this->_getNextToken();
		case ' ':
		case '\t':
			this->_skipWhitespace();
			return this->_getNextToken();
		case '\0':
			return this->_newToken("EOF", "");
		case ';':
			this->_read();
			return this->_newToken("Semicolon", ";");
		case '{':
			this->_read();
			return this->_newToken("BlockStart", "{");
		case '}':
			this->_read();
			return this->_newToken("BlockEnd", "}");
		case '#':
			return this->_scanComment();
		case '"':
		case '\'':
		case '`':
			return this->_scanQuotedString(c);
		default:
			return this->_scanKeyword();
	}
}

t_token	Lexer::scan( void )
{
	this->_latest = this->_getNextToken();
	return (this->_latest);
}

std::vector<t_token>	Lexer::all( void )
{
	std::vector<t_token> tokens;
	while (true)
	{
		t_token	token = this->scan();
		tokens.push_back(token);
		if (token.type == "EOF" || token.type == "-1")
			break;
	}
	return tokens;
}

Lexer::~Lexer() {}
