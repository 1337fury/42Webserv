/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdeel-o <abdeel-o@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/24 11:48:07 by abdeel-o          #+#    #+#             */
/*   Updated: 2023/12/21 15:48:31 by abdeel-o         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"
#include "Client.hpp"
#include "Http.hpp"
#include "Logger.hpp"

// Constructors & Destructors
Response::Response( void )
{
	_statusCode = 200;
	_status = "OK";
	_content = std::vector<char>();
	_keepAlive = false;
	_version = "1/1";
	_body = "";
	_response_string = "";
	_error = false;
}

Response::Response( Request &request, Server &server ) : 
_server(server),
_request(request)
{
	_statusCode = 200;
	_status = "OK";
	_content = std::vector<char>();
	_keepAlive = false;
	_location = _server.getMatchingLocation(_request.uri);
	_version = "1/1";
	_body = "";
	_response_string = "";
	_error = false;
}

Response::~Response( void )
{
}

Response::Response( Response const &rhs )
{
	*this = rhs;
}

Response &Response::operator=( Response const &rhs )
{
	if ( this != &rhs )
	{
		this->_statusCode = rhs._statusCode;
		this->_status = rhs._status;
		this->_headers = rhs._headers;
		this->_content = rhs._content;
		this->_request = rhs._request;
		this->_keepAlive = rhs._keepAlive;
		this->_version = rhs._version;
		this->_body = rhs._body;
		this->_response_string = rhs._response_string;
		this->_location = rhs._location;
		this->_error = rhs._error;
	}
	return *this;
}

// Getters
u_short					Response::getStatusCode( void ) const
{
	return _statusCode;
}

std::string				Response::getStatus( void ) const
{
	return _status;
}

std::vector<Header> 	Response::getHeaders( void ) const
{
	return _headers;
}

Header					Response::getHeader( std::string key ) const
{
	for ( std::vector<Header>::const_iterator it = _headers.begin(); it != _headers.end(); it++ )
	{
		if ( it->key == key )
			return *it;
	}
	return Header("", "");
}

std::vector<char> 		Response::getContent( void ) const
{
	return _content;
}

Request					Response::getRequest( void ) const
{
	return _request;
}

bool					Response::getKeepAlive( void ) const
{
	return _keepAlive;
}

std::string				Response::getVersion( void ) const
{
	return _version;
}

std::string				Response::getBody( void ) const
{
	return _body;
}

std::string				Response::getResponseString( void ) const
{
	return _response_string;
}

// Setters
void					Response::setStatusCode( u_short statusCode )
{
	_statusCode = statusCode;
}

void					Response::setStatusMessage( std::string status )
{
	_status = status;
}

void					Response::setHeaders( std::vector<Header> headers )
{
	_headers = headers;
}

void					Response::setHeader( std::string key, std::string value  )
{
	Header header("", ""); //! switch to Header header(key, value); later ...
	header.key = key;
	header.value = value;
	_headers.push_back(header);
}

void					Response::setContent( std::vector<char> content )
{
	_content = content;
}

void					Response::setRequest( Request request )
{
	_request = request;
}

void					Response::setKeepAlive( bool keepAlive )
{
	_keepAlive = keepAlive;
}

void					Response::setVersion( int major, int minor )
{
	_version = "HTTP/" + std::to_string(major) + "." + std::to_string(minor);
}

void					Response::setBody( void )
{
	_body = std::string(_content.begin(), _content.end()); // convert vector<char> to string
}

void					Response::setResponseString( void )
{
	_response_string = _version + " " + std::to_string(_statusCode) + " " + _status + "\r\n";
	for ( std::vector<Header>::iterator it = _headers.begin(); it != _headers.end(); it++ )
		_response_string += it->key + ": " + it->value + "\r\n";
	_response_string += "\r\n";
	_response_string += _body;
}

std::string 			Response::getCurrentTime() {
    time_t currentTime = time(nullptr);
    char buffer[50]; 

   	strftime(buffer, sizeof(buffer), "%a, %d %b %Y %H:%M:%S GMT", gmtime(&currentTime));
   	return  std::string(buffer);
}

void					Response::init_headers( void )
{
	_headers.push_back(Header("Server", "Nginy/1.0"));
	_headers.push_back(Header("Date", getCurrentTime()));
	_headers.push_back(Header("Last-Modified", getCurrentTime()));
	_headers.push_back(Header("Connection", _keepAlive ? "keep-alive" : "close"));
	if ( _keepAlive )
		_headers.push_back(Header("Keep-Alive", "timeout=5, max=1000")); // max=1000 means that the client can send 1000 requests before the connection is closed
}

// Methods
void 					Response::searchForErrorPage( void )
{
	std::string error_page = _server.getErrorPage(_statusCode);
	std::string content;
	if ( error_page != "" )
	{
		std::ifstream file(error_page);
		if ( file.is_open() )
		{
			while ( getline(file, content) )
				_page << content;
			file.close();
			std::string extension = error_page.substr(error_page.find_last_of(".") + 1);
			std::string mime_type = _server.getMimeType(extension);
			setHeader("Content-Type", mime_type);
			setHeader("Content-Length", std::to_string(_page.str().length()));
			content = _page.str();
			_content = std::vector<char>(content.begin(), content.end());
		}
		else
		{
			_statusCode = 500;
			_status = "Internal Server Error";
			content = "<html><body><h1>500 Internal Server Error</h1></body></html>";
			setHeader("Content-Type", _server.getMimeType("html"));
			setHeader("Content-Length", std::to_string(content.length()));
			_content = std::vector<char>(content.begin(), content.end());
		}
	}
	else // if the error page is not found
	{
		content = "<html><body><h1>EMMMMMMMMMMMMM!</h1></body></html>";
		setHeader("Content-Type", _server.getMimeType("html"));
		_content = std::vector<char>(content.begin(), content.end());
	}
}

reqStatus				Response::analyzeRequest( std::string &path )
{
	path = _request.uri;
	std::string method = _request.method;
	if (method != "GET" && method != "POST" && method != "DELETE")
		return METHOD_NOT_ALLOWED;
	if (!_location)
		return LOCATION_NOT_FOUND;
	if (_location->isRederecting())
		return LOCATIONS_IS_REDIRECTING;
	if (_location->getAcceptedMethods().size() > 0)
	{
		std::vector<std::string> acceptedMethods = _location->getAcceptedMethods();
		if (std::find(acceptedMethods.begin(), acceptedMethods.end(), _request.method) == acceptedMethods.end())
			return METHOD_NOT_ALLOWED;
	}
	// check if request content is too large 413
	if (_request.content.size() > _server.getClientBodySizeLimit())
		return REQUEST_TO_LARGE;
	path = _location->getRootDirectory() + path;
	// normalize the path
	path = normalizePath(path);
	/*[debug]*/std::cout << "path: " << path << std::endl;
	// check if the path is existing
	if (access(path.c_str(), F_OK) == -1) // Explanation: the access() function checks whether the calling process can access the file pathname. If pathname is a symbolic link, it is dereferenced. it works by checking the file's inode permission bits, not the file's mode bits. This allows the check to be made without actually attempting to open the file.
		return PATH_NOT_EXISTING;
	// check if the path is directory
	if (checks_type(path) == DIRECTORY)
		return PATH_IS_DIRECTORY;
	if (checks_type(path) == REG_FILE)
		return PATH_IS_FILE;
	return OK;
}

void					Response::create( __unused Client& client )
{
	std::string path;
	reqStatus requestStatus = analyzeRequest(path);
	switch (requestStatus)
	{
		case LOCATION_NOT_FOUND: _error = true;
			sendResponse(client.getClientSock(), 404);
			break;
		case LOCATIONS_IS_REDIRECTING:
			handleRedircetiveLocation(client.getClientSock(), _location->getRedirection());
			break;
		case METHOD_NOT_ALLOWED: _error = true;
			sendResponse(client.getClientSock(), 405);
			break;
		case REQUEST_TO_LARGE: _error = true;
			sendResponse(client.getClientSock(), 413);
			break;
		case PATH_NOT_EXISTING: _error = true;
			sendResponse(client.getClientSock(), 404);
			break;
		case PATH_IS_DIRECTORY:
			work_with_directory(client.getClientSock());
			break;
		case PATH_IS_FILE:
			work_with_file(client.getClientSock(), path);
			break;
		default:
			break;
	}
}
//  handle the setup and sending of an HTTP redirect response to the client. It performs a series of steps to set up the necessary HTTP headers, check for successful redirection, build appropriate content for the redirect page, and manage the status of the response object itself.
void					Response::handleRedircetiveLocation( __unused SOCKET clientSock, __unused Redirection redirection )
{
	Logger::getInstance().log(COLOR_CYAN, "Handling Redircetive Location...");
	if (redirection.url.find("http://", 0) != std::string::npos
		|| redirection.url.find("https://", 0) != std::string::npos)
	{
		this->setHeader("Location", redirection.url);
	}
	else if (redirection.url[0] == '/')
	{
		std::string host = _server.getHostString();
		std::string port = _server.getPortString();
		std::string url = "http://" + host + ":" + port;
		url += redirection.url;
		this->setHeader("Location", url);
	}
	else
	{
		std::string host = _server.getHostString();
		std::string port = _server.getPortString();
		std::string url = "http://" + host + ":" + port;
		url += "/" + redirection.url;
		this->setHeader("Location", url);
	}
	sendResponse(clientSock, redirection.statusCode);
}

void					Response::reset( void )
{
	_statusCode = 200;
	_status = "OK";
	_headers.clear();
	_content.clear();
	_request = Request();
	_keepAlive = false;
	_version = "";
	_body = "";
	_response_string = "";
	_location = nullptr;
	_error = false;
}

void					Response::work_with_directory(__unused SOCKET clientSock)
{
	// performs a 301 redirect if a client requests a directory without a trailing slash, adding the slash to the request URI.
	if (_request.uri[_request.uri.length() - 1] != '/')
	{
		Redirection redirection(301, _request.uri + "/");
		handleRedircetiveLocation(clientSock, redirection);
		return ;
	}
	if (_location->getDefaultFile() != "")
	{
		std::string path = _location->getRootDirectory() + _request.uri + _location->getDefaultFile();
		if (access(path.c_str(), F_OK) == -1)
		{
			_error = true;
			sendResponse(clientSock, 404);
			return ;
		}
		else
		{
			work_with_file(clientSock, path);
			return ;
		}
	}
	else if (_location->getAutoindex() && _request.method == "GET")
	{
		// if the location is autoindex then we will open the directory and read it's content and send it to the client
		DIR *dir; // pointer to a directory stream (DIR is a type representing a directory stream), it is an opaque data type representing a directory stream, it works by opening a directory stream corresponding to the directory name, and returns a pointer to the directory stream. The stream is positioned at the first entry in the directory.
		struct dirent *ent;
		std::string content, path = _location->getRootDirectory() + _request.uri;
		if ((dir = opendir(path.c_str())) != NULL)
		{
			_page << "<html><head><title>Index of " << _request.uri << "</title></head><body><h1>Index of " << _request.uri << "</h1><hr><pre>";
			while ((ent = readdir(dir)) != NULL)
				_page << "<a href=\"" << _request.uri << ent->d_name << "\">" << ent->d_name << "</a><br>";
			_page << "</pre><hr></body></html>";
			closedir(dir);
			setHeader("Content-Type", _server.getMimeType("html"));
			setHeader("Content-Length", std::to_string(_page.str().length()));
			content = _page.str();
			_content = std::vector<char>(content.begin(), content.end());
			sendResponse(clientSock, 200);
		}
		else
		{
			_error = true;
			sendResponse(clientSock, 404);
			return ;
		}
	}
	else
	{
		_error = true;
		sendResponse(clientSock, 404);
		return ;
	}
}

void					Response::work_with_file(SOCKET clientSock, std::string path)
{
	std::string	content;
	Logger::getInstance().log(COLOR_CYAN, "Working with file...");
	if (access(path.c_str(), R_OK) == -1)
	{
		_error = true;
		sendResponse(clientSock, 404);
	}
	else {
		if (_request.method == "DELETE")
		{//!DELETE
			std::cout << "Path: " << path << std::endl;
			if (remove(path.c_str()) != 0) // remove returns 0 on success and -1 on failure
				_error = true, sendResponse(clientSock, 404);
			else
			{
				setHeader("Content-Type", _server.getMimeType("html"));
				setHeader("Content-Length", std::to_string(_page.str().length()));
				content = _page.str();
				_content = std::vector<char>(content.begin(), content.end());
				sendResponse(clientSock, 200);
			}
		}
		else if (_request.method == "POST")
		{//!POST
			std::ofstream file(path);
			if (file.is_open())
			{
				file << std::string(_request.content.begin(), _request.content.end());
				file.close();
				setHeader("Content-Type", _server.getMimeType("html"));
				setHeader("Content-Length", std::to_string(_page.str().length()));
				content = _page.str();
				_content = std::vector<char>(content.begin(), content.end());
				sendResponse(clientSock, 200);
			}
			else
				_error = true, sendResponse(clientSock, 404);
		}
		else
		{//! GET
			std::ifstream file(path, std::ios::binary);
			if (file.is_open())
			{
				std::stringstream buffer;
				buffer << file.rdbuf();
				content = buffer.str();
				file.close();
				std::string extension = path.substr(path.find_last_of(".") + 1);
				std::string mime_type = _server.getMimeType(extension);
				setHeader("Content-Type", mime_type);
				setHeader("Content-Length", std::to_string(content.length()));
				_content = std::vector<char>(content.begin(), content.end());
				sendResponse(clientSock, 200);
			}
			else
				_error = true, sendResponse(clientSock, 404);
		}
	}
}

void	Response::sendResponse( SOCKET clientSock, u_short sCode )
{
	setVersion(1, 1);
	setStatusCode(sCode);
	setStatusMessage(status_code(sCode));
	setKeepAlive(false);
	init_headers();
	if (_error)
		searchForErrorPage();
	setBody();
	setResponseString();
	send(clientSock, _response_string.c_str(), _response_string.length(), 0);
	Http::closeConnection(clientSock);
	Http::removeFDFromSet(clientSock, &Http::write_set);
	reset();
}

// Example for an HTTP GET request:
// GET /static HTTP/1.1
// Host: www.example.com
// Connection: keep-alive
// Cache-Control: max-age=0
// Upgrade-Insecure-Requests: 1
// User-Agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko)
// Chrome/87.0.4280.88 Safari/537.36
// Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,
// image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9


// Example for an HTTP POST request:
// POST / HTTP/1.1
// Host: localhost
// Content-Length: 13

// name=abdelouah&age=23

// e