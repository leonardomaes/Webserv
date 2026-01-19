/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rda-cunh <rda-cunh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 19:36:46 by lmaes             #+#    #+#             */
/*   Updated: 2026/01/19 16:16:28 by rda-cunh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Header.hpp"
#include "Config.hpp"
#include "Client.hpp"
#include "Signal.hpp"
#include "Utils.hpp"

class Server
{
	private:
		int _SocketFD;
		ServerConfig _conf;
		struct sockaddr_in _SocketAddress;
	
	public:
		Server();
		Server(const Server& obj);
		~Server();

		// Functions
		Server(const ServerConfig& conf);
		void Start(Config *conf);
		int acceptClient() const;
		
		/* Defining Server Address */
		void SetAddr(int domain, int port, int interface);

		// Getters
		int getSocketFD() const;
		const ServerConfig& getConfig() const;

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
