/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdeel-o <abdeel-o@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/26 13:29:30 by abdeel-o          #+#    #+#             */
/*   Updated: 2023/12/08 09:52:53 by abdeel-o         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Header guard
#pragma once

// Libraries
#include "Lexer.hpp"
#include "Webserv.hpp"
#include "Server.hpp"

struct Block;

struct Directive {
    std::string name;
    std::vector<std::string> parameters;
	std::vector<Block> block;
};

struct Block {
	std::string name;
	std::vector<Directive> directives;
	// std::vector<Block> block;
};

class Config
{
	private:
	// Properties
		std::string					_content;
		Lexer						_lexer;
		std::vector<t_token>		_tockens;
		t_token 					_currentToken;
    	t_token 					_followingToken;
		std::vector<Server>			_servers;
	// Methods
		std::string	_getFileContent(std::string const &filename) const;
		Directive 	_parseStatement( void );
		bool		_curTokenIs(std::string type) const;
		void		_nextToken( void );
		bool		_isValidDirective(const std::string &directive, 
										std::vector<std::string> context) const;
		Block 		_parseBlock( void );
		void		_syntaxCheck( Block &block );

		void		_parseServer( Block &block );
		void		_parseLocation( std::string& path, Block &block, Server &server );

	public:
	// Constructors
		Config(std::string const &filename);
		~Config();
	// Copy constructor and assignation operator
		// Config(const Config&);
		// Config& operator=(const Config&);
	
	// Member functions
		void		parseConfig( void );

	// Getters
		std::string const &getContent() const;
		Lexer const &getLexer() const;
		std::vector<t_token> const &getTockens() const;
		std::vector<Server> const &getServers() const;

	// Exceptions
		class FileNotFound : public std::exception {
			const char* what() const throw();
		};
	// Helper functions
		void	printTokens( void );
};
