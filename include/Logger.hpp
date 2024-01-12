/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Logger.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdeel-o <abdeel-o@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 12:58:29 by abdeel-o          #+#    #+#             */
/*   Updated: 2024/01/12 18:45:28 by abdeel-o         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <stdarg.h>

#define BUFFER_SIZE 4000
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