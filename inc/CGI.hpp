/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rda-cunh <rda-cunh@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/11 20:22:39 by rda-cunh          #+#    #+#             */
/*   Updated: 2026/01/13 23:39:27 by rda-cunh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Request.hpp"
#include <map>
#include <string>
#include <cstring>
#include <iostream>
#include <vector>
#include <unistd.h>
#include <sys/wait.h>

class CGI
{
    private:
        std::map<std::string, std::string> _env;    // some people use a struct to store the variables instead of a maps
        std::string _scriptPath;                    // something in the cgi-bin dir
        std::string _interepreterPath;              // for example /usr/bin/python3 for python

        char** getEnvAsArray() const;              // converts the map of env vars into an array so that ir can be used in execve

    public:
        CGI(const std::string &scriptPath, const std::string &interpreterPath);
        ~CGI();

        std::string execute(const Request &request); // main method to execute and return a string with the output

        void initializeEnv(const Request &Request);  // initializes the environment variables
};
