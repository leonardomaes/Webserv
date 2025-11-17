/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmaes <lmaes@student.42porto.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 18:05:22 by lmaes             #+#    #+#             */
/*   Updated: 2025/11/17 18:05:23 by lmaes            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Webserv.hpp"
#include "Request.hpp"
#include "Response.hpp"

class Client
{
private:
	int			_ClientFD;
	Request		_request;
	Response	_response;
public:
	Client();
	Client(int fd, int epfd);
	~Client();

// Functions
	int readRequest(int epfd, int eventFD);

// Getter
	int getClientFD();

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
