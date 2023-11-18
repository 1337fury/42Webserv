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

#include <unistd.h>
#include <sys/stat.h>

#include <vector>
#include <map>
#include "Lexer.hpp"

#define REG_FILE 0
#define DIRECTORY 1
#define OTHER 2
#define ERROR -1

bool		isNumber(std::string str);
std::string status_code( short statusCode );
int			checks_type(std::string const path);
bool		checks_permissions(std::string const path);
bool		check_methods(std::vector<std::string>& methods);