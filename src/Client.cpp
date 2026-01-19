/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rda-cunh <rda-cunh@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 18:05:15 by lmaes             #+#    #+#             */
/*   Updated: 2026/01/18 00:24:16 by rda-cunh         ###   ########.fr       */
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

void Client::logRequest()
{
	std::cout << GREEN << "### " << _conf.listen << " ###" << std::endl
			<< "< Received Request (" << this->_request.getMethod() << " - "
			<< this->_request.getOriginalPath() << ")" << RESET << std::endl << std::endl;
}

bool Client::isChunkedRequest(const std::string& buffer)
{
	return buffer.find("Transfer-Encoding: chunked") != std::string::npos;
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

bool Client::decodeChunkedBody(const std::string& buffer, size_t body_start, std::string& out_body, size_t& consumed)
{
	size_t pos = body_start;
	out_body.clear();

	while (true)
	{
		size_t line_end = buffer.find("\r\n", pos);
		if (line_end == std::string::npos)
			return false;
		std::string size_str = buffer.substr(pos, line_end - pos);
		size_t chunk_size = strtoul(size_str.c_str(), NULL, 16);
		pos = line_end + 2;
		if (chunk_size == 0)
		{
			if (buffer.size() < pos + 2)
				return false;
			consumed = pos + 2;
			return true;
		}
		if (buffer.size() < pos + chunk_size + 2)
			return false;
		out_body.append(buffer, pos, chunk_size);
		pos += chunk_size + 2;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////// FUNCTIONS ///////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////


bool Client::readRequest(int eventFD, ServerConfig conf)
{
	char buffer[4096];
	ssize_t bytes;
	bool clientClosed = false;
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
			clientClosed = true;
			break;
		}
		break;
	}
	size_t header_end = _recvBuffer.find("\r\n\r\n");
	if (header_end == std::string::npos)
	{
		if (clientClosed)
			throw ClientException("client disconnected");
		return false;
	}

	size_t body_start = header_end + 4;
	
	// Chunked content
	if (isChunkedRequest(_recvBuffer))
	{
		std::string decodedBody;
		size_t consumed = 0;

		if (!decodeChunkedBody(_recvBuffer, body_start, decodedBody, consumed))
		{
			if (clientClosed)
				throw ClientException("client disconnected");
			return false;
		}
		std::string full_request = _recvBuffer.substr(0, body_start) + decodedBody;

		_request = Request(conf);
		_request.parseRequest(full_request);

		_recvBuffer.clear();
		logRequest();
		printMsg("(START)");
		// printMsg(full_request);
		printMsg("(END)");
		return true;
	}
	
	// Normal content
	size_t content_length = extractContentLength(_recvBuffer);
	size_t total_size = body_start + content_length;
	if (_recvBuffer.size() < total_size)
	{
		if (clientClosed)
			throw ClientException("client disconnected");
		return false;	// Incomplete body
	}
	this->_request = Request(conf);
	this->_request.parseRequest(_recvBuffer);
	printMsg("(START)");
	// printMsg(_recvBuffer);
	printMsg("(END)");
	logRequest();
	_recvBuffer.clear();
	
	return true;
}

void Client::sendResponse(int epfd, int eventFD)
{
	this->_response = Response();
	// printMsg("Body(test):" + this->_request.getBody() + "<");
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

bool Client::isKeepAlive()
{
	return (this->_request.getConnection() == "keep-alive");
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
