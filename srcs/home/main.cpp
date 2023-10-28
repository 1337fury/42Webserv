/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdeel-o <abdeel-o@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/23 12:39:26 by abdeel-o          #+#    #+#             */
/*   Updated: 2023/10/28 10:47:10 by abdeel-o         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Lexer.hpp"
#include "Config.hpp"

int	main(int argc, char *argv[])
{
    std::string configFile = "config/default_config.conf";
    if (argc == 2)
        configFile = argv[1];
	try
	{
		Config		config(configFile);
		config.printTokens();
	} catch (std::exception &e)
	{
		std::cerr << e.what() << std::endl;
		return (1);
	}
	return (0);
}