/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdeel-o <abdeel-o@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/23 12:39:26 by abdeel-o          #+#    #+#             */
/*   Updated: 2023/11/09 19:35:16 by abdeel-o         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Lexer.hpp"
#include "Config.hpp"

void	printBlock(Block block, int depth)
{
	for (int i = 0; i < depth; i++)
		std::cout << "\t";
	std::cout << "name: " << block.name << std::endl;
	for (int i = 0; i < depth; i++)
		std::cout << "\t";
	std::cout << "directives: " << block.directives.size() << std::endl;
	for (int i = 0; i < depth; i++)
		std::cout << "\t";
	std::cout << "blocks: " << block.block.size() << std::endl;
	for (int i = 0; i < depth; i++)
		std::cout << "\t";
	std::cout << "----------------" << std::endl;
	for (size_t i = 0; i < block.directives.size(); i++)
	{
		for (int j = 0; j < depth; j++)
			std::cout << "\t";
		std::cout << "name: " << block.directives[i].name << std::endl;
		for (int j = 0; j < depth; j++)
			std::cout << "\t";
		std::cout << "parameters: " << block.directives[i].parameters.size() << std::endl;
		for (int j = 0; j < depth; j++)
			std::cout << "\t";
		std::cout << "blocks: " << block.directives[i].block.size() << std::endl;
		for (int j = 0; j < depth; j++)
			std::cout << "\t";
		std::cout << "----------------" << std::endl;
		for (size_t k = 0; k < block.directives[i].block.size(); k++)
			printBlock(block.directives[i].block[k], depth + 1);
	}
	for (size_t i = 0; i < block.block.size(); i++)
		printBlock(block.block[i], depth + 1);
}

// main function
int	main(int argc, char *argv[])
{
    std::string configFile = "config/default_config.conf";
    if (argc == 2)
        configFile = argv[1];
	try
	{
		Config		config(configFile);
		// config.printTokens();
		Block block = config.parseConfig();
		printBlock(block, 0);
	} catch (std::exception &e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
		return (1);
	}
	return (0);
}