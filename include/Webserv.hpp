/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.hpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdeel-o <abdeel-o@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/26 13:35:27 by abdeel-o          #+#    #+#             */
/*   Updated: 2023/10/28 10:21:43 by abdeel-o         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <fstream>
#include <sstream>

#include <string>
#include <cstring>
#include <cstdlib>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>

#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>

#include <vector>
#include <map>
#include "Lexer.hpp"

#define REG_FILE 0
#define DIRECTORY 1
#define OTHER 2
#define ERROR -1

#define COLOR_RESET 5
#define COLOR_RED 6
#define COLOR_YELLOW 7
#define COLOR_GREEN 8
#define COLOR_BLUE 9
#define COLOR_CYAN 10
#define COLOR_GRAY 11
#define COLOR_LIGHT_GRAY 12

bool		isNumber(std::string str);
std::string status_code( u_short statusCode );
int			checks_type(std::string const path);
bool		checks_permissions(std::string const path);
bool		check_methods(std::vector<std::string>& methods);
int			set_non_blocking(int fd);
std::string normalizePath(std::string& path);
bool		supported_extension(std::string const& path);
std::string	ft_itos(size_t i);