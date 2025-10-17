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

Server::Server(const Server& obj)
{
    (void)obj;
}

Server Server::operator=(const Server& obj)
{
	if (this != &obj)
	{
		*this = obj;
	}
	return *this;
}

Server::~Server()
{
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////// FUNCTIONS ///////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

Server::Server(int port, u_long interface)
{
	// Establish Connection
	if ((this->_SocketFD = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		throw ServerException("Couldn't create socket");
	// Sets the address of the server
	SetAddr(AF_INET, port, interface);
	// Bind or Connect Socket to Address
	if (bind(this->_SocketFD, (const sockaddr*)&this->_SocketAddress, sizeof(this->_SocketAddress)) == -1)
		throw ServerException("Couldn't bind port");
	// Server will wait for connections
	if (listen(this->_SocketFD, 10) == -1)
		throw ServerException("Listen failed");


	int clientFD = accept(this->_SocketFD, (sockaddr*)&this->_SocketAddress, (socklen_t*)(sizeof(this->_SocketAddress)));
    char buffer[1024] = { 0 };
    recv(clientFD, buffer, sizeof(buffer), 0);
    std::cout << "Message from client: " << buffer
              << std::endl;

    // closing the socket.
    close(clientFD);
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

