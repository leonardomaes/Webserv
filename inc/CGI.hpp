/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rda-cunh <rda-cunh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/11 20:22:39 by rda-cunh          #+#    #+#             */
/*   Updated: 2026/01/22 10:20:49 by rda-cunh         ###   ########.fr       */
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
#include <sys/time.h>
#include <signal.h>
#include <fcntl.h>

class CGI
{
    private:
        std::map<std::string, std::string> _env;
        std::string _scriptPath;                    
        std::string _interpreterPath;              

        char** getEnvAsArray() const;               // converts the map of env vars into an array so that ir can be used in execve

    public:
        CGI(const std::string &scriptPath, const std::string &interpreterPath);
        ~CGI();

        std::string execute(const Request &request); // main method to execute and return a string with the output

        void initializeEnv(const Request &Request);  // initializes the environment variables
};
