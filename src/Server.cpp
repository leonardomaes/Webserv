/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rda-cunh <rda-cunh@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 19:36:46 by lmaes             #+#    #+#             */
/*   Updated: 2026/01/18 00:27:43 by rda-cunh         ###   ########.fr       */
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

Server::~Server()
{
	if (this->_SocketFD != -1)
		close(this->_SocketFD);
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////// FUNCTIONS ///////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

/* Defining Server Address */
void Server::SetAddr(int domain, int port, int interface)
{
	this->_SocketAddress.sin_family = domain;
	// Converts port to network byte order
	this->_SocketAddress.sin_port = htons(port);
	this->_SocketAddress.sin_addr.s_addr = interface;	// INADDR_ANY (htonl(interface))
	// INADDR_LOOPBACK: the local machine’s IP address: localhost, or 127.0.0.1
	// INADDR_ANY: the IP address 0.0.0.0
	// INADDR_BROADCAST: the IP address 255.255.255.255
}

Server::Server(const ServerConfig& conf) : _conf(conf)
{
	// Establish Connection and define it as non-block
	if ((this->_SocketFD = socket(AF_INET, SOCK_STREAM/*  | SOCK_NONBLOCK */, 0)) == -1)
		throw ServerException("Couldn't create socket");

	// Sets the address of the server
	int reusePort = 1;
	setsockopt(this->_SocketFD, SOL_SOCKET, SO_REUSEPORT, &reusePort, sizeof(reusePort));
	setsockopt(this->_SocketFD, SOL_SOCKET, SO_REUSEADDR, &reusePort, sizeof(reusePort));
	memset(&this->_SocketAddress, 0, sizeof(this->_SocketAddress));
	this->_SocketAddress.sin_family = AF_INET;
	// Converts port to network byte order
	this->_SocketAddress.sin_port = htons(std::atoi(conf.listen.c_str()));
	if (conf.host.empty() || conf.host == "0.0.0.0")
		this->_SocketAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	else
	{
		if (inet_pton(AF_INET, conf.host.c_str(), &this->_SocketAddress.sin_addr) != 1)
			throw ServerException("Invalid host IP address");
	}
	// INADDR_LOOPBACK: the local machine’s IP address: localhost, or 127.0.0.1
	// INADDR_ANY: the IP address 0.0.0.0
	// INADDR_BROADCAST: the IP address 255.255.255.255
	// Bind or Connect Socket to Address/Port
	if (bind(this->_SocketFD, (const sockaddr*)&this->_SocketAddress, sizeof(this->_SocketAddress)) == -1)
		throw ServerException("Couldn't bind port");
	// Server will listen for connections
	if (listen(this->_SocketFD, MAX_CONNECTIONS) == -1)
		throw ServerException("Listen failed");
	// Setting as non-blocking
	int flags = fcntl(this->_SocketFD, F_GETFL, 0);
	if (fcntl(this->_SocketFD, F_SETFL, flags | O_NONBLOCK) == -1)
		throw ServerException("Non-Blocking failed");
}

int Server::acceptClient() const
{
	sockaddr_in client_addr;
	socklen_t addrlen = sizeof(client_addr);

	int client_fd = accept(_SocketFD, (sockaddr*)&client_addr, &addrlen);

	if (client_fd < 0)
		return -1;

	// Non-blocking
	int flags = fcntl(client_fd, F_GETFL, 0);
	if (flags >= 0)
		fcntl(client_fd, F_SETFL, flags | O_NONBLOCK);

	return client_fd;
}

//////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////// GETTER /////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

int Server::getSocketFD() const
{
	return this->_SocketFD;
}

const ServerConfig& Server::getConfig() const
{
	return _conf;
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

