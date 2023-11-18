/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdeel-o <abdeel-o@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 11:02:29 by abdeel-o          #+#    #+#             */
/*   Updated: 2023/11/18 12:17:59 by abdeel-o         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Webserv.hpp"

class Location
{
	private:
	// Properties
		std::string 				_path;
		std::vector<std::string>	_acceptedMethods;
		std::string 				_redirection;
		std::string 				_rootDirectory;
		std::string 				_defaultFile;
		std::vector<std::string> 	_cgiExtension;
		std::vector<std::string> 	_cgiPath;
		bool						_autoindex;
		std::string 				_alias;
		unsigned long				_client_max_body_size;
		// bool 						_directoryListing;
		// bool 						_acceptUploads;
		// std::string 				_uploadDirectory;


	public:
	// Constructors
		Location( void );
		Location( Location const &rhs );
		Location &operator=( Location const &rhs );
		~Location( void );
	// Getters
		std::string 				getPath( void ) const;
		std::vector<std::string>	getAcceptedMethods( void ) const;
		std::string 				getRedirection( void ) const;
		std::string 				getRootDirectory( void ) const;
		std::string 				getDefaultFile( void ) const;
		bool						getAutoindex( void ) const;
		std::string 				getAlias( void ) const;
		std::vector<std::string>	getCgiExtension( void ) const;
		std::vector<std::string>	getCgiPath( void ) const;
		unsigned long				getClientMaxBodySize( void ) const;
		// bool 					getDirectoryListing( void ) const;
		// bool 					getAcceptUploads( void ) const;
		// std::string 				getUploadDirectory( void ) const;
	// Setters
		void		setPath( std::string path );
		void		setAcceptedMethods( std::vector<std::string> acceptedMethods );
		void		setRedirection( std::string redirection );
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