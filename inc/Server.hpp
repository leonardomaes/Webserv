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

#include "Webserv.hpp"

class Server
{
private:
	int _SocketFD;
	int	_ClientFD;
	struct sockaddr_in _SocketAddress;
public:
	Server();
	~Server();

// Functions
    Server(int port, u_long interface);
    void Start();
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
