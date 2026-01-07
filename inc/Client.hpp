/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rda-cunh <rda-cunh@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 18:05:22 by lmaes             #+#    #+#             */
/*   Updated: 2025/12/09 23:31:20 by rda-cunh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Header.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "Config.hpp"
#include "Utils.hpp"
#include "Webserv.hpp"

class Client
{
private:
	int			_ClientFD;
	Request		_request;
	Response	_response;
	ServerConfig _conf;

// Functions
public:
	Client();
	Client(const Client& obj);
	// Client& operator=(const Client& obj);
	~Client();
	Client(int fd, ServerConfig conf);

// Getter
	int getClientFD();
	ServerConfig getConfig();

// Functions
	void readRequest(int epfd, int eventFD, ServerConfig conf);
	void sendResponse(int epfd,  int eventFD);
	void closeConnection(int epfd);

// Exception
	class ClientException : public std::exception {
		private:
			std::string _errorMsg;
		public:
			ClientException(const std::string& error);
			~ClientException() throw();
			virtual const char* what() const throw();
	};
};
