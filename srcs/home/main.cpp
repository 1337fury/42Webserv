/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdeel-o <abdeel-o@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/23 12:39:26 by abdeel-o          #+#    #+#             */
/*   Updated: 2024/01/12 19:03:10 by abdeel-o         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Lexer.hpp"
#include "Config.hpp"
#include "Http.hpp"

int	main(int argc, char *argv[])
{
    std::string configFile = "config/default_config.conf";
    if (argc == 2)
        configFile = argv[1];
	try
	{
		Config			 config(configFile);
		config.parseConfig();
		Http			 http(config.getServers());
		http.initServers();
		http.run();
		
	} catch (std::exception &e)
	{
		std::cerr << e.what() << std::endl;
		return (1);
	}
	return (0);
}