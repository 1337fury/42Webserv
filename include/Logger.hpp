/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Logger.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdeel-o <abdeel-o@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 12:58:29 by abdeel-o          #+#    #+#             */
/*   Updated: 2023/11/23 13:00:54 by abdeel-o         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <sstream>
#include <iomanip>
#include <ctime>

// Singleton class : is a class that can have only one object (an instance of the class) at a time.
class Logger 
{
	public:
		static Logger& getInstance();
		void log(const std::string& message, int colorCode);

	private:
		Logger() {}
		std::string	getTimestamp( void );
		void setColor( int colorCode );
};