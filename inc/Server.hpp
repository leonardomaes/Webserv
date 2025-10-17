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
#include <csignal>
#include <cstdlib>
#include <cstring>
#include <exception>
#include <fcntl.h>
#include <fstream>
#include <limits>
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

// Webserv
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>

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

// Macros

#define MAX_EVENTS 64
#define BACKLOG 16
#define BUFFER_SIZE 4096
#define PORT 8080


class Server
{
private:
	int _SocketFD;
	int	_ClientFD;
	struct sockaddr_in _SocketAddress;
public:
	Server();
	Server(const Server& obj);
	Server operator=(const Server& obj);
	~Server();

// Functions
    Server(int port, u_long interface);
/* Defining Server Address */
	void SetAddr(int domain, int port, int interface);


// Getters
	int getClientFD();
	int getSocketFD();

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
