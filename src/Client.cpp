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

size_t Client::extractContentLength(const std::string& buffer)
{
	size_t pos = buffer.find("Content-Length:");
	if (pos == std::string::npos)
		return 0;

	pos += 15;
	while (pos < buffer.size() && buffer[pos] == ' ')
		pos++;

	return std::strtoul(buffer.c_str() + pos, NULL, 10);
}


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////// FUNCTIONS ///////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////



bool Client::readRequest(int epfd, int eventFD, ServerConfig conf)
{
	char buffer[4096];
	ssize_t bytes;
	while (true)
	{
		bytes = recv(eventFD, buffer, sizeof(buffer), 0);
		if (bytes > 0)
		{
			_recvBuffer.append(buffer, bytes);
			continue;
		}
		if (bytes == 0)
		{
			closeConnection(epfd);
			throw ClientException("client disconnected");
		}
		break;
	}
	size_t header_end = _recvBuffer.find("\r\n\r\n");
	if (header_end == std::string::npos)
		return false;
	size_t content_length = extractContentLength(_recvBuffer);
	size_t total_size = header_end + 4 + content_length;
	if (_recvBuffer.size() < total_size)
		return false;	// Incomplete body
	this->_request = Request(conf);
	this->_request.parseRequest(_recvBuffer);
	_recvBuffer.clear();
	std::cout << GREEN << "### " << _conf.listen << " ###" << std::endl
					<< "< Received Request (" << this->_request.getMethod() << " - "
					<< this->_request.getPathTarget() << ")" << RESET << std::endl;
	printMsg("(START)");
	printMsg(buffer);
	printMsg("(END)");
	return true;
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
