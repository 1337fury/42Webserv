/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdeel-o <abdeel-o@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 11:02:29 by abdeel-o          #+#    #+#             */
/*   Updated: 2023/12/14 17:23:18 by abdeel-o         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Webserv.hpp"
#include "Server.hpp"

typedef struct Redirection
{
	Redirection(u_short statusCode, std::string url) : statusCode(statusCode), url(url) {}
	u_short			statusCode;
	std::string		url;
}				Redirection;

class Location
{
	private:
	// Properties
		std::string 						_path;
		std::vector<std::string>			_acceptedMethods;
		std::vector<std::string> 			_redirection; //! change type to std::vector<std::string>
		std::string 						_rootDirectory;
		std::string 						_defaultFile;
		std::vector<std::string> 			_cgiExtension;
		std::vector<std::string> 			_cgiPath;
		bool								_autoindex;
		std::string 						_alias;
		unsigned long						_client_max_body_size;
		// bool 						_directoryListing;
		// bool 						_acceptUploads;
		// std::string 				_uploadDirectory;
		bool								_isRederecting;


	public:
	// Constructors
		Location( void );
		Location( Location const &rhs );
		Location &operator=( Location const &rhs );
		~Location( void );
	// Getters
		std::string 				getPath( void ) const;
		std::vector<std::string>	getAcceptedMethods( void ) const;
		Redirection 				getRedirection( void ) const;
		std::string 				getRootDirectory( void );
		std::string 				getDefaultFile( void ) const;
		bool						getAutoindex( void ) const;
		std::string 				getAlias( void ) const;
		std::vector<std::string>	getCgiExtension( void ) const;
		std::vector<std::string>	getCgiPath( void ) const;
		unsigned long				getClientMaxBodySize( void ) const;
		// bool 					getDirectoryListing( void ) const;
		// bool 					getAcceptUploads( void ) const;
		// std::string 				getUploadDirectory( void ) const;
		bool						isRederecting( void ) const;
	// Setters
		void		setPath( std::string path );
		void		setAcceptedMethods( std::vector<std::string> acceptedMethods );
		void		setRedirection( std::vector<std::string> red ); //! change arg type to std::vector<std::string>
		void		setRootDirectory( std::string rootDirectory );
		void		setDefaultFile( std::string defaultFile );
		void		setAutoindex( bool autoindex );
		void		setAlias( std::string alias );
		void		setCgiExtension( std::vector<std::string> cgiExtension );
		void		setCgiPath( std::vector<std::string> cgiPath );
		void		setClientMaxBodySize( std::string clientBodySizeLimit );
		// void		setDirectoryListing( bool directoryListing );
		// void		setAcceptUploads( bool acceptUploads );
		// void		setUploadDirectory( std::string uploadDirectory );
};