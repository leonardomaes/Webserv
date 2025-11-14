/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rda-cunh <rda-cunh@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 18:55:08 by rda-cunh          #+#    #+#             */
/*   Updated: 2025/11/14 18:55:08 by rda-cunh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <fstream>

class Config
{
    private: 
        std::string _path;

    public:
        Config(const std::string path);
        bool parseConfig();     //parse config file and return true if sucess
};