/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rda-cunh <rda-cunh@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 19:36:46 by lmaes             #+#    #+#             */
/*   Updated: 2025/12/09 23:09:21 by rda-cunh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Webserv.hpp"
#include "Config.hpp"
#include "Client.hpp"
#include "Signal.hpp"
#include "Utils.hpp"

class Server
{
private:
	int _SocketFD;
	Client _clients[MAX_CONNECTIONS];
	struct sockaddr_in _SocketAddress;
public:
	Server();
	Server(const Server& obj);
	// Server& operator=(const Server& obj);
	~Server();

// Functions
    Server(int port, u_long interface);
    void Start(Config *conf);
/* Defining Server Address */
	void SetAddr(int domain, int port, int interface);


// Getters
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
