/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdeel-o <abdeel-o@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 11:02:10 by abdeel-o          #+#    #+#             */
/*   Updated: 2024/01/15 19:01:05 by abdeel-o         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"

Location::Location( void ) {
	this->_path = "";
	this->_acceptedMethods = std::vector<std::string>();
	this->_redirection = std::vector<std::string>();
	this->_rootDirectory = "";
	this->_defaultFile = "";
	this->_acceptUploads = false;
	this->_uploadDirectory = "";
	this->_autoindex = false;
	this->_cgiPath = "";
	this->_isRederecting = false;
	this->_isCgi = false;
	this->heapAllocated = false;
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
		this->_defaultFile = rhs._defaultFile;
		this->_acceptUploads = rhs._acceptUploads;
		this->_uploadDirectory = rhs._uploadDirectory;
		this->_autoindex = rhs._autoindex;
		this->_cgiPath = rhs._cgiPath;
		this->_isRederecting = rhs._isRederecting;
		this->_isCgi = rhs._isCgi;
		this->heapAllocated = rhs.heapAllocated;
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
Redirection 				Location::getRedirection( void ) const {
	Redirection redirection(atoi(this->_redirection[0].c_str()), this->_redirection[1]);
	return redirection;
}
std::string 				Location::getRootDirectory( void ) {
	if (this->_rootDirectory.back() == '/')
		this->_rootDirectory.pop_back();
	return this->_rootDirectory;
}
std::string 				Location::getDefaultFile( void ) const {
	return this->_defaultFile;
}
bool						Location::getAutoindex( void ) const {
	return this->_autoindex;
}
std::string					Location::getCgiPath( void ) const {
	return this->_cgiPath;
}
bool						Location::isCgi( void ) const {
	return this->_isCgi;
}
bool 						Location::getAcceptUploads( void ) const {
	return this->_acceptUploads;
}
std::string 				Location::getUploadDirectory( void ) const {
	return this->_uploadDirectory;
}
bool						Location::isRederecting( void ) const {
	return this->_isRederecting;
}

void		Location::setPath( std::string path ) {
	if (path[0] != '/')
		path = "/" + path;
	if (path == "/cgi-bin")
		this->_isCgi = true;
	this->_path = path;
}
void		Location::setAcceptedMethods( std::vector<std::string> acceptedMethods ) {
	check_methods(acceptedMethods);
	this->_acceptedMethods = acceptedMethods;
}

void		Location::setRedirection( std::vector<std::string> redirection) {
	if (redirection[0].length() != 3 || !isNumber(redirection[0]))
		throw std::invalid_argument("nginy: [location] redirection directive invalid value");
	if (atoi(redirection[0].c_str()) < 301 || atoi(redirection[0].c_str()) > 302)
		throw std::invalid_argument("nginy: [location] the redirection directive only accepts 301 and 302");
	_isRederecting = true;
	this->_redirection = redirection;
}

void		Location::setRootDirectory( std::string rootDirectory ) {
	if (checks_type(rootDirectory) != DIRECTORY)
		throw std::invalid_argument("nginy: Invalid root directory");
	this->_rootDirectory = rootDirectory;
}
void		Location::setDefaultFile( std::string defaultFile ) {
	this->_defaultFile = defaultFile;
}
void		Location::setAutoindex( bool autoindex ) {
	this->_autoindex = autoindex;
}
void		Location::setCgiPath( std::string cgiPath ) {
	if ( checks_type(cgiPath) != REG_FILE)
		throw std::invalid_argument("nginy: cgi path is not valid");
	this->_cgiPath = cgiPath;
}
void		Location::setAcceptUploads( bool acceptUploads ) {
	this->_acceptUploads = acceptUploads;
}
void		Location::setUploadDirectory( std::string uploadDirectory ) {
	if ( checks_type(uploadDirectory) != DIRECTORY)
		throw std::invalid_argument("nginy: upload directory is not valid");
	this->_uploadDirectory = uploadDirectory;
}
void		Location::setCgi( bool isCgi ) {
	this->_isCgi = isCgi;
}