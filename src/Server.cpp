/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rda-cunh <rda-cunh@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 19:36:46 by lmaes             #+#    #+#             */
/*   Updated: 2025/12/09 23:30:51 by rda-cunh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Server.hpp"

Server::Server()
{

}

Server::Server(const Server& obj)
{
	_SocketFD = obj._SocketFD;
	_SocketAddress = obj._SocketAddress;
}

// Server& Server::operator=(const Server& obj)
// {
// 	if (this != &obj)
// 	{
// 		*this = obj;
// 	}
	
// 	return *this;
// }

Server::~Server()
{
	if (this->_SocketFD != -1)
        close(this->_SocketFD);
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

void	Server::Start(Config *conf)
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

	while (g_running) //RM: using g_running instead of 1
	{
		struct epoll_event events[MAX_EVENTS];
		// Epoll stays till a event
		// Then proccess n events
		int eventsReady = epoll_wait(epfd, events, MAX_EVENTS, -1);
		if (eventsReady < 0)
		{
			if (errno == EINTR) //RM: if interrupted by a signal, ignore and go to the beggining of the loop (calling epoll_wait)
				continue;
			else
				throw ServerException("epoll_wait failed");
		}
		for (int i = 0; i < eventsReady; i++)
		{
			int fd = events[i].data.fd;
			if (fd == this->_SocketFD)
			{
		 		socklen_t addrlen = sizeof(this->_SocketAddress);
				int client_fd = accept(this->_SocketFD, (sockaddr*)&this->_SocketAddress, &addrlen);
				if (client_fd < 0)
					continue;
				this->_clients[client_fd] = Client(client_fd, epfd, conf->getServerConfig(0));	// Change 0 to index server TO DO
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
					std::cerr << "LOG::Client::" << e.what() << '\n';
				}
			}
		}
	}
	// Cleanup after loop exits
    std::cout << "\nShutting down gracefully\n" << std::endl;
    close(epfd);
    close(this->_SocketFD);

	// Close connection to clients
	for (int it = 0; it < MAX_CONNECTIONS; it++)
	{
		_clients[it].closeConnection(epfd);
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

