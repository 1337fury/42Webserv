/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdeel-o <abdeel-o@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/26 13:40:10 by abdeel-o          #+#    #+#             */
/*   Updated: 2023/11/12 11:07:45 by abdeel-o         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"
#include <Server.hpp>

Config::Config(std::string const &filename) :
_content(_getFileContent(filename)),
_lexer(Lexer(_content)),
_tockens(_lexer.all()),
_currentToken(),
_followingToken()
{
	_serverdir.push_back("listen");
	_serverdir.push_back("server_name");
	_serverdir.push_back("default_error_page");
	_serverdir.push_back("client_body_size_limit");
	_serverdir.push_back("location");
	_routedir.push_back("path");
	_routedir.push_back("accepted_methods");
	_routedir.push_back("redirection");
	_routedir.push_back("root_directory");
	_routedir.push_back("directory_listing");
	_routedir.push_back("default_file");
	_routedir.push_back("cgi_extension");
	_routedir.push_back("accept_uploads");
	_routedir.push_back("upload_directory");
}

std::string	Config::_getFileContent(std::string const &filename) const {
	std::ifstream file(filename);
    if (!file.is_open())
		throw Config::FileNotFound();

    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();

    return buffer.str();
}

// Getters
std::string const& Config::getContent() const {
	return _content;
}
Lexer const& Config::getLexer() const {
	return _lexer;
}
std::vector<t_token> const& Config::getTockens() const {
	return _tockens;
}

// Exceptions
const char* Config::FileNotFound::what() const throw() {
	return "File not found or permission denied";
}

Config::~Config() {}


// Helper functions
void	Config::printTokens( void )
{
	std::vector<t_token>::iterator it = _tockens.begin();
	for (; it  != _tockens.end(); it++)
		std::cout << it->type << " " << it->literal << std::endl;
}

bool Config::_isValidDirective(const std::string &directive, std::vector<std::string> context) const {
	return std::find(context.begin(), context.end(), directive) != context.end();
}

bool	Config::_curTokenIs(std::string type) const
{
	return _currentToken.type == type;
}

void	Config::_nextToken( void ) 
{
	_currentToken 	= _followingToken;
	if (_tockens.empty())
	{
		_currentToken.type = "EOF";
		_currentToken.literal = "";
		return ;
	}
	_followingToken = _tockens.front();
	_tockens.erase(_tockens.begin());
}

Directive Config::_parseStatement( void )
{
	Directive directive;
	directive.name = _currentToken.literal;
	
	while (true)
	{
		_nextToken();
		if (_curTokenIs("Keyword"))
		{
			directive.parameters.push_back(_currentToken.literal);
		}
		else
			break;
	}
	if (_curTokenIs("Semicolon"))
		return directive;
	if (_curTokenIs("BlockStart"))
	{
		_nextToken();
		directive.block.push_back(_parseBlock());
		directive.block[0].name = directive.name + " block";
	}

	return directive;
}

/*
	Comment server 1
	Keyword server
	BlockStart {
	Comment listen
	Keyword listen
	Keyword 80
	Keyword 90
	Semicolon ;
	BlockEnd }
*/

Block	Config::_parseBlock()
{
	Block context;
	Directive statement;

	while (true) {
		if (_curTokenIs("BlockEnd") || _curTokenIs("EOF")) {
			break;
		} else if (_curTokenIs("Keyword")) {
			statement = _parseStatement();
			context.directives.push_back(statement);
		}
		if (_curTokenIs("EOF"))
			throw std::runtime_error("WebServ: unexpected end of file, expecting `}`"); //handle error: missing block end }
		else if (_curTokenIs("BlockStart"))
			throw std::runtime_error("WebServ: unexpected `{`"); //handle error: {{
		
		_nextToken();
	}
    return context;
}

Block	Config::parseConfig( void ) {
	Block root;
	_nextToken();
	_nextToken();
    root = _parseBlock();
	_syntaxCheck(root);
	return root;
}

void	Config::_syntaxCheck( Block &root )
{
	if (root.directives.size() == 0)
		throw std::runtime_error("WebServ: No server found");
	for (size_t i = 0; i < root.directives.size(); i++)
	{
		if (root.directives[i].name != "server")
			throw std::runtime_error("WebServ: unknown directive or wrong scope");
		if (!root.directives[i].block.size())
			throw std::runtime_error("WebServ: Server must have a block");
		if (root.directives[i].parameters.size() != 0)
			throw std::runtime_error("WebServ: Server must not have parameters");
	}
	// to be continued ...
}
