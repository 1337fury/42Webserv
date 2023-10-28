/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdeel-o <abdeel-o@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/26 13:29:30 by abdeel-o          #+#    #+#             */
/*   Updated: 2023/10/28 12:24:23 by abdeel-o         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Header guard
#pragma once

// Libraries
#include "Webserv.hpp"
#include "Lexer.hpp"

class Config
{
	private:
	// Properties
		std::string				_content;
		Lexer					_lexer;
		std::vector<t_token>	_tockens;
	// Methods
		std::string	_getFileContent(std::string const &filename) const;

	public:
	// Constructors
		Config(std::string const &filename);
		~Config();
	// Copy constructor and assignation operator
		// Config(const Config&);
		// Config& operator=(const Config&);

	// Getters
		std::string const &getContent() const;
		Lexer const &getLexer() const;
		std::vector<t_token> const &getTockens() const;
		
	// Exceptions
		class FileNotFound : public std::exception {
			const char* what() const throw();
		};
	// Helper functions
		void	printTokens( void );
};
