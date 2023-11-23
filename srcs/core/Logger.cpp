/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Logger.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdeel-o <abdeel-o@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 12:36:36 by abdeel-o          #+#    #+#             */
/*   Updated: 2023/11/23 15:48:17 by abdeel-o         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Logger.hpp"
Logger& Logger::getInstance() {
	static Logger instance;
	return instance;
}

void Logger::log(const std::string& message, int colorCode = 0) {
	setColor(colorCode);
	std::cout << getTimestamp() << " " << message << std::endl;
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
		case 1: color = "\033[1;31m"; break; // Red
		case 2: color = "\033[1;32m"; break; // Green
		default: color = "\033[0m"; break;   // Default (reset)
	}
	std::cout << color;
}
