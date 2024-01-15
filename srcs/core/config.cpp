/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdeel-o <abdeel-o@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/26 13:40:10 by abdeel-o          #+#    #+#             */
/*   Updated: 2024/01/15 18:41:09 by abdeel-o         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"
#include <Server.hpp>
#include "Location.hpp"

Config::Config(std::string const &filename) :
_content(_getFileContent(filename)),
_lexer(Lexer(_content)),
_tockens(_lexer.all()),
_currentToken(),
_followingToken(),
_reservedPorts()
{
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
std::vector<Server> const& Config::getServers() const {
	
	for (size_t i = 0; i < _servers.size(); i++)
	{
		if (_servers[i].getPort() == 0)
			throw std::runtime_error("nginy: server must have a listen directive");
		if (_servers[i].getRoot() == "")
			throw std::runtime_error("nginy: server must have a root directive");
	}
	return _servers;
}


const char* Config::FileNotFound::what() const throw() {
	return "File not found or permission denied";
}

Config::~Config() {}



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
			throw std::runtime_error("nginy: unexpected end of file, expecting `}`");
		else if (_curTokenIs("BlockStart"))
			throw std::runtime_error("nginy: unexpected `{`");
		
		_nextToken();
	}
    return context;
}

void		Config::parseConfig( void ) {
	_nextToken();
	_nextToken();
    Block root = _parseBlock();
	root.name = "root";
	_syntaxCheck(root);
	size_t i = 0;
	while (i < root.directives.size())
	{
		_parseServer(root.directives[i].block[0]);
		i++;
	}
}

void	Config::_syntaxCheck( Block &root )
{
	if (root.directives.size() == 0)
		throw std::runtime_error("nginy: No server found");
	for (size_t i = 0; i < root.directives.size(); i++)
	{
		if (root.directives[i].name != "server")
			throw std::runtime_error("nginy: unknown directive or wrong scope");
		if (!root.directives[i].block.size())
			throw std::runtime_error("nginy: Server must have a block");
		if (root.directives[i].parameters.size() != 0)
			throw std::runtime_error("nginy: Server must not have parameters");
	}
	
}

void Config::_parseServer( Block &block )
{
	Server server;
	std::vector<Directive> blockDirectives = block.directives;
	bool	 bodySizeDone = false;
	bool	 indexDone = false;

	for (size_t i = 0; i < blockDirectives.size(); i++)
	{
		if (blockDirectives[i].name == "listen")
		{
			if (server.getPort() != 0)
				throw std::runtime_error("nginy: listen directive is duplicate");
			if (blockDirectives[i].parameters.size() != 1)
				throw std::runtime_error("nginy: invalid number of arguments in `listen`");
			server.setPort(blockDirectives[i].parameters[0]);
			if (std::find(_reservedPorts.begin(), _reservedPorts.end(), blockDirectives[i].parameters[0]) != _reservedPorts.end())
				throw std::runtime_error("nginy: port " + blockDirectives[i].parameters[0] + " is reserved");
			_reservedPorts.push_back(blockDirectives[i].parameters[0]);
		}
		else if (blockDirectives[i].name == "host")
		{
			if (server.getHost() != 0)
				throw std::runtime_error("nginy: host directive is duplicate");
			if (blockDirectives[i].parameters.size() != 1)
				throw std::runtime_error("nginy: invalid number of arguments in `host`");
			server.setHost(blockDirectives[i].parameters[0]);
		}
		else if (blockDirectives[i].name == "root")
		{
			if (server.getRoot() != "")
				throw std::runtime_error("nginy: root directive is duplicate");
			if (blockDirectives[i].parameters.size() != 1)
				throw std::runtime_error("nginy: invalid number of arguments in `root`");
			server.setRoot(blockDirectives[i].parameters[0]);
		}
		else if (blockDirectives[i].name == "error_page")
		{
			if (blockDirectives[i].parameters.size() < 2)
				throw std::runtime_error("nginy: invalid number of arguments in `error_page`");
			server.setErrorPage(blockDirectives[i].parameters);
		}
		else if (blockDirectives[i].name == "client_max_body_size")
		{
			if (bodySizeDone)
				throw std::runtime_error("nginy: client_max_body_size directive is duplicate");
			if (blockDirectives[i].parameters.size() != 1)
				throw std::runtime_error("nginy: invalid number of arguments in `client_max_body_size`");
			server.setClientBodySizeLimit(blockDirectives[i].parameters[0]);
			bodySizeDone = true;
		}
		else if (blockDirectives[i].name == "server_name")
		{
			if (blockDirectives[i].parameters.size() != 1)
				throw std::runtime_error("nginy: invalid number of arguments in `server_name`");
			if (server.getServerNames().size() != 0)
				throw std::runtime_error("nginy: server_name directive is duplicate");
			server.setServerNames(blockDirectives[i].parameters);
		}
		else if (blockDirectives[i].name == "index")
		{
			if (blockDirectives[i].parameters.size() != 1)
				throw std::runtime_error("nginy: invalid number of arguments in `index`");
			if (server.getIndex() != "")
				throw std::runtime_error("nginy: index directive must be unique");
			server.setIndex(blockDirectives[i].parameters[0]);
		}
		else if (blockDirectives[i].name == "autoindex")
		{
			if (blockDirectives[i].parameters.size() != 1)
				throw std::runtime_error("nginy: invalid number of arguments in `autoindex`");
			if (indexDone)
				throw std::runtime_error("nginy: autoindex directive is duplicate");
			if (blockDirectives[i].parameters[0] == "on")
				server.setAutoindex(true);
			else if (blockDirectives[i].parameters[0] == "off")
				server.setAutoindex(false);
			else
				throw std::runtime_error("nginy: invalid argument in `autoindex`");
			indexDone = true;
		}
		else if (blockDirectives[i].name == "method")
		{
			if (blockDirectives[i].parameters.size() < 1)
				throw std::runtime_error("nginy: invalid number of arguments in `methods`");
			if (server.getAcceptedMethods().size() != 0)
				throw std::runtime_error("nginy: methods directive is duplicate");
			server.setAcceptedMethods(blockDirectives[i].parameters);
		}
		else if (blockDirectives[i].name == "location")
		{
			if (blockDirectives[i].parameters.size() != 1)
				throw std::runtime_error("nginy: invalid number of arguments in `location`");
			_parseLocation(blockDirectives[i].parameters[0] ,blockDirectives[i].block[0], server);
		}
		else
			throw std::runtime_error("nginy: unknown directive `" + blockDirectives[i].name + "`");
	}
	
	_servers.push_back(server);
}

void		Config::_parseLocation( std::string& path, Block &block, Server &server )
{
	Location	location;
	std::vector<Directive> blockDirectives = block.directives;
	bool	 autoindexDone = false;
	
	location.setPath(path);
	for (size_t i = 0; i < blockDirectives.size(); i++)
	{
		if (blockDirectives[i].name == "root")
		{
			if (location.getRootDirectory() != "")
				throw std::runtime_error("nginy: [location] root directive is duplicate");
			if (blockDirectives[i].parameters.size() != 1)
				throw std::runtime_error("nginy: [location] invalid number of arguments in `root`");
			location.setRootDirectory(blockDirectives[i].parameters[0]);
		}
		else if (blockDirectives[i].name == "method")
		{
			if (location.getAcceptedMethods().size() != 0)
				throw std::runtime_error("nginy: [location] methods directive is duplicate");
			if (blockDirectives[i].parameters.size() < 1)
				throw std::runtime_error("nginy: [location] invalid number of arguments in `method`");
			location.setAcceptedMethods(blockDirectives[i].parameters);
		}
		else if (blockDirectives[i].name == "autoindex")
		{
			if (blockDirectives[i].parameters.size() != 1)
				throw std::runtime_error("nginy: [location] invalid number of arguments in `autoindex`");
			if (autoindexDone)
				throw std::runtime_error("nginy: [location] autoindex directive is duplicate");
			if (blockDirectives[i].parameters[0] == "on")
				location.setAutoindex(true);
			else if (blockDirectives[i].parameters[0] == "off")
				location.setAutoindex(false);
			else
				throw std::runtime_error("nginy: [location] invalid argument in `autoindex`");
			autoindexDone = true;
		}
		else if (blockDirectives[i].name == "index")
		{
			if (location.getDefaultFile() != "")
				throw std::runtime_error("nginy: [location] index directive is duplicate");
			if (blockDirectives[i].parameters.size() != 1)
				throw std::runtime_error("nginy: [location] invalid number of arguments in `index`");
			location.setDefaultFile(blockDirectives[i].parameters[0]);
		}
		else if (blockDirectives[i].name == "return")
		{
			if (location.isCgi()) 
				throw std::runtime_error("nginy: [location] return directive not allowed in cgi-bin"); 
			if (location.isRederecting())
				throw std::runtime_error("nginy: [location] return directive is duplicate");
			if (blockDirectives[i].parameters.size() != 2) 
				throw std::runtime_error("nginy: [location] invalid number of arguments in `return`");
			location.setRedirection(blockDirectives[i].parameters);
		}
		else if (blockDirectives[i].name == "cgi_path")
		{
			if (!location.isCgi())
				throw std::runtime_error("nginy: [location] cgi_path directive not allowed in this location: " + path);
			if (location.getCgiPath().size() != 0)
				throw std::runtime_error("nginy: [location] cgi_path directive is duplicate");
			if (blockDirectives[i].parameters.size() != 1)
				throw std::runtime_error("nginy: [location] invalid number of arguments in `cgi_path`");
			location.setCgiPath(blockDirectives[i].parameters[0]);
		}
		else if (blockDirectives[i].name == "upload_path")
		{
			if (location.isCgi())
				throw std::runtime_error("nginy: [location] upload_path directive not allowed in this location: " + path);
			if (location.getUploadDirectory().size() != 0)
				throw std::runtime_error("nginy: [location] upload_path directive is duplicate");
			if (blockDirectives[i].parameters.size() != 1)
				throw std::runtime_error("nginy: [location] invalid number of arguments in `upload_path`");
			location.setAcceptUploads(true);
			location.setUploadDirectory(blockDirectives[i].parameters[0]);
		}
		else
			throw std::runtime_error("nginy: [location] unknown directive `" + blockDirectives[i].name + "`");
	}
	if (location.getRootDirectory() == "")
		location.setRootDirectory(server.getRoot());
	if (!location.isCgi() && location.getDefaultFile() == "")
		location.setDefaultFile(server.getIndex());
	if (!autoindexDone)
		location.setAutoindex(server.getAutoindex());
	if (location.getAcceptedMethods().size() == 0)
		location.setAcceptedMethods(server.getAcceptedMethods());
	
	server.setLocation(location);
}
