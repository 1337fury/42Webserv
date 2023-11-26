/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Logger.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdeel-o <abdeel-o@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 12:58:29 by abdeel-o          #+#    #+#             */
/*   Updated: 2023/11/25 16:21:23 by abdeel-o         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <stdarg.h>

#define BUFFER_SIZE 4000
// Singleton class : is a class that can have only one object (an instance of the class) at a time.
class Logger 
{
	public:
		static Logger& getInstance();
		void log(int colorCode, const char *format, ...);

	private:
		Logger() {}
		std::string	getTimestamp( void );
		void setColor( int colorCode );
};