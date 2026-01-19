/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rda-cunh <rda-cunh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 19:36:46 by lmaes             #+#    #+#             */
/*   Updated: 2026/01/19 16:22:38 by rda-cunh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Webserv.hpp"
#include "../inc/Config.hpp"

void printStartupHeader()
{
    std::cout
        << "========================================\n"
        << "            WEBSERV SERVER               \n"
        << "----------------------------------------\n"
        << "  HTTP/1.1 compatible web server        \n"
        << "  Written in C++98                      \n"
        << "  School 42                             \n"
        << "========================================\n"
        << std::endl;
}

int main(int ac, char **av)
{
    printStartupHeader();

    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);

    try
    {
        std::string configPath;
        configPath = (ac > 1) ? av[1] : "default.conf";
        Config config(configPath);

        Webserv engine(&config);
		engine.run();
    }
    catch(const std::exception& e)
    {
        std::cerr << "Server::" << e.what() << '\n';
    }
    
    return 0;
}
