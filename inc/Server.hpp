/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmaes <lmaes@student.42porto.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 19:36:46 by lmaes             #+#    #+#             */
/*   Updated: 2025/10/06 19:36:47 by lmaes            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <algorithm>
#include <cctype>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <cstring>
#include <exception>
#include <fstream>
#include <limits>
#include <iostream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <sstream>
#include <sys/time.h>
#include <bits/stdc++.h>
#include <string>
#include <unistd.h>

// Containers
#include <deque>
#include <vector>
#include <stack>
#include <list>
#include <map>

// Colors
#define RESET   "\033[0m"
#define BLACK   "\033[30m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"

// Background colors
#define BCK_BLACK   "\033[40m"
#define BCK_RED     "\033[41m"
#define BCK_GREEN   "\033[42m"
#define BCK_YELLOW  "\033[43m"
#define BCK_BLUE    "\033[44m"
#define BCK_MAGENTA "\033[45m"
#define BCK_CYAN    "\033[46m"
#define BCK_WHITE   "\033[47m"

class Server
{
private:
    /* data */
public:
    Server();
	Server(const Server& obj);
	Server operator=(const Server& obj);
    ~Server();




    // Exception
	class ServerException : public std::exception {
		private:
			std::string _errorMsg;
		public:
			ServerException(const std::string& error);
			~ServerException() throw();
			virtual const char* what() const throw();
	};
};
