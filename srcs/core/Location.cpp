/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdeel-o <abdeel-o@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 11:02:10 by abdeel-o          #+#    #+#             */
/*   Updated: 2023/12/11 11:48:45 by abdeel-o         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"

Location::Location( void ) {
	this->_path = "";
	this->_acceptedMethods = std::vector<std::string>();
	this->_redirection = "";
	this->_rootDirectory = "";
	// this->_directoryListing = false;
	this->_defaultFile = "";
	// this->_acceptUploads = false;
	// this->_uploadDirectory = "";
	this->_autoindex = false;
	this->_alias = "";
	this->_cgiExtension = std::vector<std::string>();
	this->_cgiPath = std::vector<std::string>();
	this->_client_max_body_size = 0;
}

Location::Location( Location const &rhs ) {
	*this = rhs;
}

Location &Location::operator=( Location const &rhs ) {
	if ( this != &rhs ) {
		this->_path = rhs._path;
		this->_acceptedMethods = rhs._acceptedMethods;
		this->_redirection = rhs._redirection;
		this->_rootDirectory = rhs._rootDirectory;
		// this->_directoryListing = rhs._directoryListing;
		this->_defaultFile = rhs._defaultFile;
		// this->_acceptUploads = rhs._acceptUploads;
		// this->_uploadDirectory = rhs._uploadDirectory;
		this->_autoindex = rhs._autoindex;
		this->_alias = rhs._alias;
		this->_cgiExtension = rhs._cgiExtension;
		this->_cgiPath = rhs._cgiPath;
		this->_client_max_body_size = rhs._client_max_body_size;
	}
	return *this;
}

Location::~Location( void ) {
}

// Getters
std::string 				Location::getPath( void ) const {
	return this->_path;
}
std::vector<std::string>	Location::getAcceptedMethods( void ) const {
	return this->_acceptedMethods;
}
std::string 				Location::getRedirection( void ) const {
	return this->_redirection;
}
std::string 				Location::getRootDirectory( void ) const {
	return this->_rootDirectory; // root directory is the directory that contains the files that we want to serve, example: /var/www/html
}
std::string 				Location::getDefaultFile( void ) const {
	return this->_defaultFile; // default file is the file that we want to serve if the client requests a directory, example: index.html, the directive name is index
}
std::vector<std::string>	Location::getCgiExtension( void ) const {
	return this->_cgiExtension;
}
bool						Location::getAutoindex( void ) const {
	return this->_autoindex;
}
std::string 				Location::getAlias( void ) const {
	return this->_alias;
}
std::vector<std::string>	Location::getCgiPath( void ) const {
	return this->_cgiPath;
}
unsigned long				Location::getClientMaxBodySize( void ) const {
	return this->_client_max_body_size;
}
// bool 						Location::getDirectoryListing( void ) const {
// 	return this->_directoryListing;
// }
// bool 						Location::getAcceptUploads( void ) const {
// 	return this->_acceptUploads;
// }
// std::string 				Location::getUploadDirectory( void ) const {
// 	return this->_uploadDirectory;
// }
// Setters
void		Location::setPath( std::string path ) {
	this->_path = path;
}
void		Location::setAcceptedMethods( std::vector<std::string> acceptedMethods ) {
	// check if methods are valid
	check_methods(acceptedMethods);
	this->_acceptedMethods = acceptedMethods;
}
void		Location::setRedirection( std::string redirection ) {
	this->_redirection = redirection;
}
void		Location::setRootDirectory( std::string rootDirectory ) {
	if (checks_type(rootDirectory) != DIRECTORY)
		throw std::invalid_argument("WebServ: Invalid root directory");
	this->_rootDirectory = rootDirectory;
}
void		Location::setDefaultFile( std::string defaultFile ) {
	this->_defaultFile = defaultFile;
}
void		Location::setAutoindex( bool autoindex ) {
	this->_autoindex = autoindex;
}
void		Location::setAlias( std::string alias ) {
	this->_alias = alias;
}
void		Location::setCgiPath( std::vector<std::string> cgiPath ) {
	this->_cgiPath = cgiPath;
}
void		Location::setCgiExtension( std::vector<std::string> cgiExtension ) {
	this->_cgiExtension = cgiExtension;
}
void	Location::setClientMaxBodySize( std::string clientBodySizeLimit ) {
	if (clientBodySizeLimit.length() > 10 || !isNumber(clientBodySizeLimit))
		throw std::runtime_error("WebServ: [location] client_max_body_size directive invalid value");
	this->_client_max_body_size = atoi(clientBodySizeLimit.c_str());
}
// void		Location::setDirectoryListing( bool directoryListing ) {
// 	this->_directoryListing = directoryListing;
// }
// void		Location::setAcceptUploads( bool acceptUploads ) {
// 	this->_acceptUploads = acceptUploads;
// }
// void		Location::setUploadDirectory( std::string uploadDirectory ) {
// 	this->_uploadDirectory = uploadDirectory;
// }