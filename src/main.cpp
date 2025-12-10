/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rda-cunh <rda-cunh@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 19:36:46 by lmaes             #+#    #+#             */
/*   Updated: 2025/11/14 18:54:45 by rda-cunh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Server.hpp"
#include "../inc/Config.hpp"

int main(int ac, char **av)
{

    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);

    try
    {
        std::string configPath;
        configPath = (ac > 1) ? av[1] : "default.conf";
        Config config(configPath);

        Server serv(8080, INADDR_ANY);
		serv.Start(config);
    }
    catch(const std::exception& e)
    {
        std::cerr << "Server::" << e.what() << '\n';
    }
    
    return 0;
}
