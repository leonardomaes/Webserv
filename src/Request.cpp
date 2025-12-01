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
		{
			// Error case (TO ADD)
			// Code will assume HTTP code
		}
	}
}

int Request::parseFirstLine(std::string line)
{
	this->_firstLine = 0;
	int i = 0;
	while (line[i] != ' ')
		this->_method.insert(_method.end(), line[i++]);
	i++;
	if (_method != "GET" && _method != "POST" && _method != "DELETE")
		return 400;
	while (line[i] != ' ')
		this->_pathTarget.insert(_pathTarget.end(), line[i++]);
	i++;
	// If _pathTarget is invalid (Parsing target path) then error 404
	// TO DO
	while (line[i] != '\r')
		this->_protocol.insert(_protocol.end(), line[i++]);
	if (_protocol != "HTTP/1.1")
		return 400;
	// std::cout << "DBG::" << _protocol << "(protocol)" << std::endl;
	return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////// GETTER /////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

std::string Request::getConnection()
{
	return _connection;
}

