/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rda-cunh <rda-cunh@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 18:55:22 by rda-cunh          #+#    #+#             */
/*   Updated: 2025/11/14 18:55:22 by rda-cunh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Config.hpp"
#include "../inc/Webserv.hpp"

Config::Config(const std::string path) : _path(path) { parseConfig(); }

bool Config::parseConfig()
{
    std::ifstream file(_path.c_str());
    if (!file.is_open())
    {
        std::cerr << "Could not open config file: " << _path << std::endl;
        return (false);
    }

    std::string line;
    int lineNum = 1;
    while (std::getline(file, line))     //simple version to print lines of the config file. This will evolve later into something bigger
    {
        std::cout << "Line " << lineNum << ": " << line << std::endl;
        lineNum++;
    }
    return (true);

}