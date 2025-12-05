/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmaes <lmaes@student.42porto.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 17:26:07 by lmaes             #+#    #+#             */
/*   Updated: 2025/10/27 17:26:07 by lmaes            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Request.hpp"

Request::Request() : _method(""), _pathTarget(""), _protocol(""), _firstLine(1)
{
	_connection = "close";
}

// Request::Request(std::string buffer)
// {

// }

Request::~Request()
{
}

int Request::parsePath()		// TO DO   (Config File)
{
	if (this->_pathTarget == "/")
		this->_pathTarget = "/index.html";
	else if (this->_pathTarget == "/favicon.ico")
		return 0;
	else
	{
		this->_pathTarget = "/error/404.html"; // path = locateError (TO DO)
		return 1;
	}
	return 0;
}

int Request::parseFirstLine(std::string line)
{
	int code = 200;
	this->_firstLine = 0;
	int i = 0;
	while (line[i] != ' ')
		this->_method.insert(_method.end(), line[i++]);
	i++;
	// If _method is invalid , then invalid Method
	if (_method != "GET" && _method != "POST" && _method != "DELETE")
		code = 405;
	while (line[i] != ' ')
		this->_pathTarget.insert(_pathTarget.end(), line[i++]);
	i++;
	// If _pathTarget is invalid (Parsing target path), then invalid page
	if (this->parsePath() != 0 && code == 200)
		code = 404;

	while (line[i] != '\r')
		this->_protocol.insert(_protocol.end(), line[i++]);
	if (_protocol != "HTTP/1.1" && code == 200)
		code = 400;
	// std::cout << "DBG::" << _protocol << "(protocol - 1)" << std::endl;
	return code;
}


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////// FUNCTIONS ///////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

void Request::parseRequest(std::string buffer)
{
	std::string line;
	std::istringstream request(buffer);
	int code = 0;
	size_t header_end = buffer.find("\r\n\r\n");
	if (header_end == std::string::npos)
		code = 400;
	while (std::getline(request, line))
	{
		if (this->_firstLine)
			code = this->parseFirstLine(line);
		if (code != 0)
			break ;
	}
	this->_responseCode = code;
}

//////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////// GETTER /////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

std::string Request::getConnection()
{
	return _connection;
}

std::string Request::getPathTarget()
{
	return _pathTarget;
}

std::string Request::getHost()
{
	return _host;
}

int Request::getCode()
{
	return _responseCode;
}

std::string Request::getProtocol()
{
	return _protocol;
}


