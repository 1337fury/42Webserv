/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Lexer.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdeel-o <abdeel-o@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/12 09:58:55 by abdeel-o          #+#    #+#             */
/*   Updated: 2023/10/28 10:45:04 by abdeel-o         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Header guard
#pragma once

// Libraries
#include <iostream>
#include <vector>

// token types
typedef struct s_token
{
	std::string	type;
	std::string	literal;
	int			line;
	int 		column;
}	t_token;

class Lexer
{
	private:
	// Properties
		std::string _content;
		int			_line;
		int			_position;
		int 		_column;
		t_token		_latest;
	// Default constructor
		Lexer( void );

	public:
	// Constructors
		Lexer( std::string const &content );
		~Lexer( void );
	// Public Methods
		t_token	scan( void );
		std::vector<t_token> all( void );
	
	private:
	// Private Methods
		t_token	_getNextToken( void ); 
		char	_peek( void );
		void	_read( void );
		void	_skipWhitespace( void );
		t_token _newToken( std::string type, std::string literal );
		t_token _scanComment( void );
		t_token _scanQuotedString( char delimiter );
		t_token _scanKeyword( void );
};
