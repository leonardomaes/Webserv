/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rda-cunh <rda-cunh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 18:05:22 by lmaes             #+#    #+#             */
/*   Updated: 2026/01/19 09:46:57 by rda-cunh         ###   ########.fr       */
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
		std::string	_recvBuffer;
		Response	_response;
		ServerConfig _conf;
		
		// Functions
		size_t	extractContentLength(const std::string& buffer);
		bool	isChunkedRequest(const std::string& buffer);
		bool	decodeChunkedBody(const std::string& buffer, size_t body_start, std::string& out_body, size_t& consumed);
		void	logRequest();
	public:
		Client();
		Client(const Client& obj);
		~Client();
		Client(int fd, ServerConfig conf);

		// Getter
		int getClientFD();
		ServerConfig getConfig();

		// Functions
		bool readRequest(int eventFD, ServerConfig conf);
		void sendResponse(int epfd,  int eventFD);
		void closeConnection(int epfd);
		bool	isKeepAlive();

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
