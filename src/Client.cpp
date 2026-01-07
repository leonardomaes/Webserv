/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rda-cunh <rda-cunh@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 18:05:15 by lmaes             #+#    #+#             */
/*   Updated: 2025/12/09 23:34:37 by rda-cunh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Client.hpp"

Client::Client() : _ClientFD(0)
{
}

Client::Client(const Client &obj)
{
	_ClientFD = obj._ClientFD;
	_request = obj._request;
	_response = obj._response;
}

// Client& Client::operator=(const Client& obj)
// {
// 	if (this != &obj)
// 	{
// 		*this = obj;
// 	}
	
// 	return *this;
// }

Client::~Client()
{
}

Client::Client(int fd, ServerConfig conf) : _ClientFD(fd), _request(conf), _response(), _conf(conf)
{
	if (_ClientFD < 0)
		throw ClientException("Accept failed");
	// Setting client as non-blocking
	int flags = fcntl(this->_ClientFD, F_GETFL, 0);
	if (fcntl(this->_ClientFD, F_SETFL, flags | O_NONBLOCK) == -1)
		throw ClientException("Non-Blocking failed");
}

void Client::closeConnection(int epfd)
{
	if (_ClientFD <= 0)		// If already closed
		return;

	// Remove FD from epoll
	epoll_ctl(epfd, EPOLL_CTL_DEL, _ClientFD, NULL);

	// Close socket
	close(_ClientFD);

	//Mark as closed
	_ClientFD = -1;

}


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////// FUNCTIONS ///////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////



void Client::readRequest(int epfd, int eventFD, ServerConfig conf)
{
	this->_request = Request(conf);
	char buffer[1024];
	// Received HTTP
	ssize_t bytes = recv(eventFD, buffer, sizeof(buffer) - 1, 0);
	if ((bytes <= 0))	// Error(-1) or closed by EOF (0)
	{
// RM: I sugest to delete this commented part bellow and do it inside closeConnection(epfd);
//		close(eventFD);
//		epoll_ctl(epfd, EPOLL_CTL_DEL, eventFD, NULL);
		closeConnection(epfd);
		throw ClientException("Read failed");
	}
	buffer[bytes] = '\0';
	this->_request.parseRequest(buffer);
	std::cout << GREEN << "### " << _conf.listen << " ###" << std::endl << "< Received Request (" << this->_request.getMethod() << " - "
				<< this->_request.getPathTarget() << ")" << RESET << std::endl;		// LOG
	// TO DO
	// Parse of HTTP Request (REQUEST)
	printMsg("(START)");
	printMsg(buffer);
	printMsg("(END)");
}

void Client::sendResponse(int epfd, int eventFD)
{

	this->_response = Response();
	printMsg("Body(test):" + this->_request.getBody() + "<");
	this->_response.generateResponse(this->_request, epfd, eventFD);
}

//////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////// GETTER /////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////


int Client::getClientFD()
{
	return this->_ClientFD;
}

ServerConfig Client::getConfig()
{
	return this->_conf;
}

//////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////// EXCEPTIONS ///////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

Client::ClientException::ClientException(const std::string& error)
{
	_errorMsg = "Error: " + error;
}

Client::ClientException::~ClientException() throw() {}


const char *Client::ClientException::what() const throw() {
	return _errorMsg.c_str();
}
