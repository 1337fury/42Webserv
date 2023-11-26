/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Logger.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdeel-o <abdeel-o@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 12:36:36 by abdeel-o          #+#    #+#             */
/*   Updated: 2023/11/25 16:33:36 by abdeel-o         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Logger.hpp"
#include "Webserv.hpp"

Logger& Logger::getInstance() {
	static Logger instance;
	return instance;
}

void Logger::log(int colorCode, const char *format, ...) {
	char buffer[BUFFER_SIZE];
	va_list args;
	va_start(args, format);
	vsnprintf(buffer, BUFFER_SIZE, format, args);
	setColor(colorCode);
	std::cout << getTimestamp() << " ";
	std::cout << buffer << std::endl;
}

std::string	Logger::getTimestamp( void )
{
	std::time_t	currentTime;
	std::tm*	localTime;
	char		buffer[20];

	std::time(&currentTime);
	localTime = std::localtime(&currentTime);
	std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S ", localTime);
	return (buffer);
}

void Logger::setColor( int colorCode )
{
	const char* color = "";
	switch (colorCode) {
		case COLOR_RED: color = "\x1b[31m"; break; // Red
		case COLOR_GREEN: color = "\x1b[32m"; break; // Green
		case COLOR_YELLOW: color = "\x1b[33m"; break; // Yellow
		case COLOR_BLUE: color = "\x1b[34m"; break; // Blue
		case COLOR_CYAN: color = "\x1b[36m"; break; // Cyan
		case COLOR_GRAY: color = "\x1b[90m"; break; // Gray
		case COLOR_LIGHT_GRAY: color = "\x1b[37m"; break; // Light Gray
		default: color = "\x1b[0m"; break;   // Default (reset)
	}
	std::cout << color;
}
