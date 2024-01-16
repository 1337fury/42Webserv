/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdeel-o <abdeel-o@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/24 11:48:07 by abdeel-o          #+#    #+#             */
/*   Updated: 2024/01/16 16:02:04 by abdeel-o         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"
#include "Client.hpp"
#include "Http.hpp"
#include "Logger.hpp"

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
	_location = nullptr;
	_cgi_pid = -1;
	_headers = std::vector<Header>();
	_cgi_stdout = -1;
	_cgi_stderr = -1;
	_cgi = false;
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
	_cgi_pid = -1;
	_headers = std::vector<Header>();
	_cgi_stdout = -1;
	_cgi_stderr = -1;
	_cgi = false;
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
		this->_cgi_pid = rhs._cgi_pid;
		this->_server = rhs._server;
		this->_cgi_stdout = rhs._cgi_stdout;
		this->_cgi_stderr = rhs._cgi_stderr;
		this->_cgi = rhs._cgi;
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
	Header header("", "");
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
	_body = std::string(_content.begin(), _content.end());
}

void					Response::setResponseString( void )
{
	_response_string = _version + " " + std::to_string(_statusCode) + " " + _status + "\r\n";
	for ( std::vector<Header>::iterator it = _headers.begin(); it != _headers.end(); it++ )
		_response_string += it->key + ": " + it->value + "\r\n";
	if (!_cgi || _error)
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
		_headers.push_back(Header("Keep-Alive", "timeout=5, max=1000"));
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
	else
	{
		Logger::getInstance().log(COLOR_RED, "Error page not found for status code: %d", _statusCode);
		content = "<html><body><h1><center>OOPS!</center></h1><hr></hr><center>nginy/1.0</center></body></html>";
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
	std::vector<std::string> acceptedMethods = _location->getAcceptedMethods();
	if (std::find(acceptedMethods.begin(), acceptedMethods.end(), _request.method) == acceptedMethods.end())
		return METHOD_NOT_ALLOWED;
	if (_request.content.size() > _server.getClientBodySizeLimit())
		return REQUEST_TO_LARGE;
	if (_location->getAcceptUploads() && _request.method == "POST")
		return LOCATION_IS_UPLOADING;
	path = _location->getRootDirectory() + path;
	// normalize the path
	path = normalizePath(path);
	if (access(path.c_str(), F_OK) == -1) 
		return PATH_NOT_EXISTING;
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
			createResponse(client.getClientSock(), 404);
			break;
		case LOCATIONS_IS_REDIRECTING:
			Logger::getInstance().log(COLOR_GRAY, "Location is redirecting...");
			handleRedircetiveLocation(client.getClientSock(), _location->getRedirection());
			break;
		case LOCATION_IS_UPLOADING:
			handle_upload(client.getClientSock());
			break;
		case METHOD_NOT_ALLOWED: _error = true;
			createResponse(client.getClientSock(), 405);
			break;
		case REQUEST_TO_LARGE: _error = true;
			createResponse(client.getClientSock(), 413);
			break;
		case PATH_NOT_EXISTING: _error = true;
			createResponse(client.getClientSock(), 404);
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

void					Response::handleRedircetiveLocation( __unused SOCKET clientSock, __unused Redirection redirection )
{
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
	createResponse(clientSock, redirection.statusCode);
}

void					Response::reset( void )
{
	_statusCode = 200;
	_status = "OK";
	_headers.clear();
	_content.clear();
	_request = Request();
	_keepAlive = false;
	_version.clear();
	_body.clear();
	_response_string.clear();
	if (_location->heapAllocated)
		delete _location;
	_location = nullptr;
	_error = false;
	_cgi_pid = -1;
	_cgi_stdout = -1;
	_cgi_stderr = -1;
	_cgi = false;
	_page.str("");
	_page.clear();
}

void					Response::work_with_directory(__unused SOCKET clientSock)
{
	if (_request.uri[_request.uri.length() - 1] != '/')
	{
		Logger::getInstance().log(COLOR_GRAY, "Performs a 301 redirect...");
		Redirection redirection(301, _request.uri + "/");
		handleRedircetiveLocation(clientSock, redirection);
		return ;
	}
	if (_location->getDefaultFile() != "")
	{
		std::string path = _location->getRootDirectory() + _request.uri + _location->getDefaultFile();
		if (access(path.c_str(), F_OK) == -1)
		{
			Logger::getInstance().log(COLOR_GRAY, "File not found...");
			_error = true;
			createResponse(clientSock, 404);
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
		DIR *dir; 
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
			createResponse(clientSock, 200);
		}
		else
		{
			Logger::getInstance().log(COLOR_GRAY, "File not found...");
			_error = true;
			createResponse(clientSock, 404);
			return ;
		}
	}
	else
	{
		_error = true;
		createResponse(clientSock, 403);
		return ;
	}
}

void					Response::work_with_file(SOCKET clientSock, std::string path)
{
	if (access(path.c_str(), F_OK) == -1)
	{
		Logger::getInstance().log(COLOR_GRAY, "File not found...");
		_error = true;
		createResponse(clientSock, 404);
	}
	else if (_location->isCgi())
	{
		_cgi = true;
		Logger::getInstance().log(COLOR_GRAY, "CGI processing...");
		if (!supported_extension(path))
		{
			Logger::getInstance().log(COLOR_RED, "CGI File extension not supported...");
			_error = true;
			createResponse(clientSock, 501);
			return ;
		}
		if (access(path.c_str(), R_OK) == -1)
		{
			Logger::getInstance().log(COLOR_GRAY, "CGI File error...");
			_error = true;
			createResponse(clientSock, 401);
			return ;
		}
		CGI cgi(*_location, path);
		initiate_cgi_response(clientSock, cgi);
		if (!cgi.execute(_request))
		{
			Logger::getInstance().log(COLOR_RED, "CGI Not executed: `%s`", cgi.getErrorMsg().c_str());
			_error = true;
			createResponse(clientSock, 404);
			return ;
		}
		if (cgi.wait() < 0)
		{
			Logger::getInstance().log(COLOR_RED, "CGI Process error: `%s`", cgi.getErrorMsg().c_str());
			_error = true;
			createResponse(clientSock, 500);
			return ;
		}
		Logger::getInstance().log(COLOR_GRAY, "CGI Is executed...");
		_cgi_stdout = cgi.getStdout();
		_cgi_stderr = cgi.getStderr();
		_cgi_pid = cgi.getPid();
		cgiHandler(clientSock);
	}
	else
		handleFileRequest(clientSock, path);
}

void					Response::initiate_cgi_response( SOCKET clientSock, CGI &cgi )
{
	if (!cgi.validate())
	{
		Logger::getInstance().log(COLOR_RED, "CGI Not validated: `%s`", cgi.getErrorMsg().c_str());
		_error = true;
		createResponse(clientSock, 404);
		return ;
	}
	if (!cgi.setCgiEnvs(_request))
	{
		Logger::getInstance().log(COLOR_RED, "CGI Not set: `%s`", cgi.getErrorMsg().c_str());
		_error = true;
		createResponse(clientSock, 404);
		return ;
	}
	if (!cgi.setupFiles())
	{
		Logger::getInstance().log(COLOR_RED, "CGI Not setup: `%s`", cgi.getErrorMsg().c_str());
		_error = true;
		createResponse(clientSock, 404);
		return ;
	}
}


void					Response::handleFileRequest( SOCKET clientSock, std::string path )
{
	std::string	content;
	if (_request.method == "DELETE")
	{
		if (remove(path.c_str()) != 0)
			_error = true, createResponse(clientSock, 404);
		else
		{
			setHeader("Content-Type", _server.getMimeType("html"));
			setHeader("Content-Length", std::to_string(_page.str().length()));
			content = _page.str();
			_content = std::vector<char>(content.begin(), content.end());
			createResponse(clientSock, 204);
		}
	}
	else if (_request.method == "POST")
	{
		if (access(path.c_str(), F_OK) == -1)
			_error = true, createResponse(clientSock, 404);
		else
		{
			std::ofstream file(path);
			if (file.is_open())
			{
				file << std::string(_request.content.begin(), _request.content.end());
				file.close();
				createResponse(clientSock, 204);
			}
			else
				_error = true, createResponse(clientSock, 500);
		}
	}
	else
	{
		std::ifstream file(path, std::ios::in);
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
			createResponse(clientSock, 200);
		}
		else
			_error = true, createResponse(clientSock, 404);
	}
}

void					Response::cgiHandler( SOCKET clientSock )
{
	std::stringstream buffer;
	lseek(_cgi_stdout, 0, SEEK_SET);
	while (true)
	{
		char c;
		int ret = read(_cgi_stdout, &c, 1);
		if (ret == -1)
		{
			Logger::getInstance().log(COLOR_RED, "Error reading from CGI stdout");
			close(_cgi_stdout);
			close(_cgi_stderr);
			createResponse(clientSock, 500);
			return ;
		}
		else if (ret == 0)
			break ;
		buffer << c;
	}
	std::string content = buffer.str();
	_content = std::vector<char>(content.begin(), content.end());
	createResponse(clientSock, 200);
	close(_cgi_stdout);
	close(_cgi_stderr);
}

void					Response::createResponse( __unused SOCKET clientSock, u_short sCode )
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
}

void					Response::handle_upload( __unused SOCKET clientsock )
{
	Logger::getInstance().log(COLOR_YELLOW, "Handling upload...");
	std::string boundary = _request.getHeader("Content-Type").substr(30);
	std::pair<std::string, std::string> data = getData(_request.content, boundary);
	std::string body = data.first;
	std::string filename = data.second;
	
	std::ofstream file(filename);
	if (file.is_open())
	{
		file << body;
		file.close();
	}
	else
	{
		Logger::getInstance().log(COLOR_RED, "Error opening file...");
		_error = true;
		createResponse(clientsock, 500);
		return ;
	}
	std::string uploads_dir = _location->getUploadDirectory();
	std::string path = uploads_dir + (uploads_dir[uploads_dir.length() - 1] == '/' ? "" : "/") + filename;
	if (rename(filename.c_str(), path.c_str()) != 0)
	{
		Logger::getInstance().log(COLOR_RED, "Error moving file...");
		_error = true;
		createResponse(clientsock, 500);
		return ;
	}
	Logger::getInstance().log(COLOR_YELLOW, "Sending upload response...");
	createResponse(clientsock, 204);
}

std::pair<std::string, std::string>	Response::getData( __unused std::vector<char> content, __unused std::string boundary )
{
	std::string clean_body;
	std::stringstream ssbody;
	std::string body = std::string(content.begin(), content.end());
	std::string line;
	std::string filename;
	ssbody << body;

	bool boundary_line = false;
	bool content_line = false;
	for (size_t i = 0; i < body.size(); i++)
	{
		line.clear();
		std::getline(ssbody, line);
		i += line.size();
		if (!line.compare("--" + boundary + "--\r"))
		{
			content_line = true;
			boundary_line = false;
		}
		if (!line.compare("--" + boundary + "\r"))
		{
			boundary_line = true;
		}
		if (boundary_line)
		{
			if (!line.compare(0, 31, "Content-Disposition: form-data;"))
			{
				size_t pos = line.find("filename=\"");
				if (pos != std::string::npos)
				{
					filename = line.substr(pos + 10);
					filename = filename.substr(0, filename.find("\""));
				}
			}
			else if (!line.compare(0, 1, "\r") && !filename.empty())
			{
				boundary_line = false;
				content_line = true;
			}
		}
		else if (content_line)
		{
			if (!line.compare(("--" + boundary + "\r")))
			{
				boundary_line = true;
			}
			else if (!line.compare(("--" + boundary + "--\r")))
			{
				clean_body.erase(clean_body.end() - 1);
				break ;
			}
			else
			{
				clean_body += (line + "\n");
			}
		}
	}
	return std::pair<std::string, std::string>(clean_body, filename);
}

void	Response::update(int bytes_sent )
{
	_response_string.erase(0, bytes_sent);
}