/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmaes <lmaes@student.42porto.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 19:36:46 by lmaes             #+#    #+#             */
/*   Updated: 2025/10/06 19:36:47 by lmaes            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Server.hpp"

Server::Server()
{

}

Server::~Server()
{

}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////// FUNCTIONS ///////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

Server::Server(int port, u_long interface)
{
	// Establish Connection and define it as non-block
	if ((this->_SocketFD = socket(AF_INET, SOCK_STREAM/*  | SOCK_NONBLOCK */, 0)) == -1)
		throw ServerException("Couldn't create socket");

	// Sets the address of the server
	int reusePort = 1;
	setsockopt(this->_SocketFD, SOL_SOCKET, SO_REUSEPORT, &reusePort, sizeof(reusePort));
	memset(&this->_SocketAddress, 0, sizeof(this->_SocketAddress));
	SetAddr(AF_INET, port, interface);

	// Bind or Connect Socket to Address/Port
	if (bind(this->_SocketFD, (const sockaddr*)&this->_SocketAddress, sizeof(this->_SocketAddress)) == -1)
		throw ServerException("Couldn't bind port");

}

/* Defining Server Address */
void Server::SetAddr(int domain, int port, int interface)
{
	this->_SocketAddress.sin_family = domain;
	// Converts port to network byte order
	this->_SocketAddress.sin_port = htons(port);
	this->_SocketAddress.sin_addr.s_addr = htonl(interface);	// INADDR_ANY
	// INADDR_LOOPBACK: the local machine’s IP address: localhost, or 127.0.0.1
	// INADDR_ANY: the IP address 0.0.0.0
	// INADDR_BROADCAST: the IP address 255.255.255.255
}

void	Server::Start()
{
	// Server will listen for connections
	if (listen(this->_SocketFD, MAX_CONNECTIONS) == -1)
		throw ServerException("Listen failed");

	// Setting as non-blocking
	int flags = fcntl(this->_SocketFD, F_GETFL, 0);
	if (fcntl(this->_SocketFD, F_SETFL, flags | O_NONBLOCK) == -1)
		throw ServerException("Non-Blocking failed");
	// Create epoll
	int epfd = epoll_create(MAX_CONNECTIONS); // (MAX + 1)??
	// Epoll warn new reads/client with EPOLLIN
	struct epoll_event ev;
	memset(&ev, 0, sizeof(ev));
	ev.events = EPOLLIN;
	ev.data.fd = this->_SocketFD;
	if (epoll_ctl(epfd, EPOLL_CTL_ADD, this->_SocketFD, &ev) == -1)
		throw ServerException("epoll_ctl: SocketFD");

	while (1)
	{
		struct epoll_event events[MAX_EVENTS];
		// Epoll stays till a event
		// Then proccess n events
		int eventsReady = epoll_wait(epfd, events, MAX_EVENTS, -1);
		if (eventsReady < 0)
			throw ServerException("epoll_wait failed");
		for (int i = 0; i < eventsReady; i++)
		{
			int fd = events[i].data.fd;
			if (fd == this->_SocketFD)
			{
		 		socklen_t addrlen = sizeof(this->_SocketAddress);
				int client_fd = accept(this->_SocketFD, (sockaddr*)&this->_SocketAddress, &addrlen);
				if (client_fd < 0)
					continue;
				this->_clients[client_fd] = Client(client_fd, epfd);
				continue;
			}
			else if (events[i].events & EPOLLIN)
			{
				try
				{
					this->_clients[fd].readRequest(epfd, events[i].data.fd);
					this->_clients[fd].sendResponse(epfd, events[i].data.fd);
				}
				catch(const std::exception& e)
				{
					std::cerr << "Client::" << e.what() << '\n';
				}
			}
			
		}
	}
}
		//
		// 					  HEADER
		//		{HTTP/Version Status Status-Message}
		//		{Date: Fri, 16 Mar 2018 17:36:27 GMT}
		//				{Server: *Server Name*}
		// 		{Content-Type: text/html;charset=UTF-8}
		//				{Content-Length: 1846}
		//
		// 					 BLANK LINE
		//
		// 					    BODY
		// 					{<?xml ...>}
		// 				{<?CODTYPE html ...>}
		//					{<html ...>}
		//					   {...}
		//					  {</html>}
		// 
		//			HTTP/1.1 200 OK\n
		//		Content-Type: text/plain\n
		//		   Content-Length: 14\n
		//					\n
		//			  Hello world!!!
		// 
		// 

//////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////// GETTER /////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

int Server::getSocketFD()
{
	return this->_SocketFD;
}

//////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////// EXCEPTIONS ///////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

Server::ServerException::ServerException(const std::string& error)
{
	_errorMsg = "Error: " + error;
}

Server::ServerException::~ServerException() throw() {}


const char *Server::ServerException::what() const throw() {
	return _errorMsg.c_str();
}

