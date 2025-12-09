/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmaes <lmaes@student.42porto.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 18:05:15 by lmaes             #+#    #+#             */
/*   Updated: 2025/11/17 18:05:17 by lmaes            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Client.hpp"

Client::Client() : _ClientFD(0)
{
}

Client::Client(int fd, int epfd) : _ClientFD(fd), _request(), _response()
{
	if (_ClientFD < 0)
		throw ClientException("Accept failed");
	// Setting client as non-blocking
	int flags = fcntl(this->_ClientFD, F_GETFL, 0);
	if (fcntl(this->_ClientFD, F_SETFL, flags | O_NONBLOCK) == -1)
		throw ClientException("Non-Blocking failed");
	// Epoll monitor client and stays till event
	// EPOLLIN
	// EPOLLET - Each trigger remove the actual event
	epoll_event client_ev;
	client_ev.events = EPOLLIN /* | EPOLLET */;
	client_ev.data.fd = this->_ClientFD;
	if (epoll_ctl(epfd, EPOLL_CTL_ADD, this->_ClientFD, &client_ev) == -1)
		throw ClientException("epoll_ctl: ClientFD");
}

Client::~Client()
{
}


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////// FUNCTIONS ///////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////



void Client::readRequest(int epfd, int eventFD)
{
	char buffer[1024];
	// Received HTTP
	ssize_t bytes = recv(eventFD, buffer, sizeof(buffer) - 1, 0);
	if ((bytes <= 0))	// Error(-1) or closed by EOF (0)
	{
		close(eventFD);
		epoll_ctl(epfd, EPOLL_CTL_DEL, eventFD, NULL);
		throw ClientException("Read failed");
	}
	buffer[bytes] = '\0';
	this->_request = Request();
	this->_request.parseRequest(buffer);
	std::cout << "LOG:: " << GREEN << "< Received Request (" << this->_request.getMethod() << " - "
				<< this->_request.getPathTarget() << ")" << RESET << std::endl;		// LOG
	// TO DO
	// Parse of HTTP Request (REQUEST)
	std::cout << "(START)" << std::endl;		// DELETE
	std::cout << buffer << std::endl;
	std::cout << "(END)" << std::endl;						// DELETE
}

void Client::sendResponse(int epfd, int eventFD)
{

	this->_response = Response();
	this->_response.generateResponse(this->_request, epfd, eventFD);
}

//////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////// GETTER /////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////


int Client::getClientFD()
{
	return this->_ClientFD;
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


