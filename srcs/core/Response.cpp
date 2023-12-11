/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdeel-o <abdeel-o@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/24 11:48:07 by abdeel-o          #+#    #+#             */
/*   Updated: 2023/12/11 17:20:54 by abdeel-o         ###   ########.fr       */
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
	}
	return *this;
}

// Getters
u_short			Response::getStatusCode( void ) const
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
	_response_string += "\r\n";
	_response_string += _body;
}

std::string Response::getCurrentTime() {
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
void Response::searchForErrorPage( void )
{
	std::string error_page = _server.getErrorPage(_statusCode);
	if ( error_page != "" )
	{
		std::ifstream file(error_page);
		if ( file.is_open() )
		{
			std::string line;
			while ( getline(file, line) )
				_page << line;
			file.close();
			_body = _page.str();
			_content = std::vector<char>(_body.begin(), _body.end());
		}
		else
		{
			_statusCode = 500;
			_status = "Internal Server Error";
			_body = "<html><body><h1>500 Internal Server Error</h1></body></html>";
			_content = std::vector<char>(_body.begin(), _body.end());
		}
	}
	else
	{
		_statusCode = 500;
		_status = "Internal Server Error";
		_body = "<html><body><h1>500 Internal Server Error</h1></body></html>";
		_content = std::vector<char>(_body.begin(), _body.end());
	}
}

reqStatus	Response::analyzeRequest( void )
{
	std::string path = _request.uri;
	
	if (!_location)
		return LOCATION_NOT_FOUND;
	return OK;
}

void	Response::create( __unused Client& client )
{
	reqStatus requestStatus = analyzeRequest();
	switch (requestStatus)
	{
		case LOCATION_NOT_FOUND:
			sendMethodNotAllowedResponse(client.getClientSock(), 404);
			break;
		default:
			break;
	}
}

/*void	Response::create( Client& client )
{
	analyzeRequest();
	if ( _request.method == "GET" )
		goto getResponse;
	else if ( _request.method == "POST" )
		goto postResponse;
	else if ( _request.method == "DELETE" )
		goto deleteResponse;
	else
		goto methodNotAllowedResponse;

	getResponse:
		handleGETrequest(client.getClientSock());
	postResponse:
		return ;
	deleteResponse:
		return ;
	methodNotAllowedResponse:
		sendMethodNotAllowedResponse(client.getClientSock());
}*/

void	Response::reset( void )
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
}

// response
void	Response::sendMethodNotAllowedResponse( SOCKET clientSock, u_short statusCode )
{
	Logger::getInstance().log(COLOR_CYAN, "Sending Method Not Allowed Response");
	setVersion(1, 1);
	setStatusCode(statusCode);
	setStatusMessage(status_code(statusCode));
	setKeepAlive(false);
	init_headers();	
	searchForErrorPage();
	setBody();
	setResponseString();
	send(clientSock, _response_string.c_str(), _response_string.length(), 0);
	Http::closeConnection(clientSock);
	Http::removeFDFromSet(clientSock, &Http::write_set);
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

// process request and send appropriate response
void	Response::handleGETrequest( __unused SOCKET clientSock )
{
	Logger::getInstance().log(COLOR_CYAN, "Responding to GET request...");
	std::string uri = _request.uri;
	
	Http::closeConnection(clientSock);
	Http::removeFDFromSet(clientSock, &Http::write_set);
}