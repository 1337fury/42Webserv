/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Logger.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdeel-o <abdeel-o@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 12:36:36 by abdeel-o          #+#    #+#             */
/*   Updated: 2024/01/03 19:09:50 by abdeel-o         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Logger.hpp"
#include "Webserv.hpp"

Logger& Logger::getInstance() {
	static Logger instance; // Guaranteed to be destroyed because it's a static variable and it's destroyed when main() finishes executing (or when exit() is called). when you call getInstance() for the first time, the instance is created. When you call getInstance() subsequently, the existing instance is returned.
	// we use a static variable to make sure that only one instance of the class exists at a time. because static variables are allocated storage in static storage area, and only one copy of the static variables exists.
	// if we don't use a static variable, the instance will be created on the stack, and a new instance will be created each time we call getInstance(). and we will have multiple instances of the class.
	return instance;
}

void Logger::log(int colorCode, const char *format, ...) {
	char buffer[BUFFER_SIZE] = {0};
	va_list args;
	va_start(args, format);
	vsnprintf(buffer, BUFFER_SIZE, format, args);
	va_end(args);
	setColor(colorCode);
	std::cout << std::flush;
	std::cout << getTimestamp() << " " << buffer << std::endl;
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
