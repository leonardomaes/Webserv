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
	_errorPage[400] = "/error/400.html";		// Config File
	_errorPage[401] = "/error/401.html";
	_errorPage[403] = "/error/403.html";
	_errorPage[404] = "/error/404.html";
	_errorPage[405] = "/error/405.html";
}

// Request::Request(std::string buffer)
// {

// }

Request::~Request()
{
}

int Request::fileOpen(std::string target)
{
	std::string filename = "./assets/html";	// Config file
	filename.append(target);
	std::ifstream file(filename.c_str(), std::ios::in);
	if (!file.is_open())
		return 0;
	file.close();
	return 1;
}

int Request::parsePath()		// TO DO   (Config File)
{
	if (this->_pathTarget == "/")					// Default case
		this->_pathTarget = "/index.html";					// _pathTarget = _indexFile (config.hpp)
	else if (this->_pathTarget == "/favicon.ico")	// Special case
	{
		if (this->_method != "GET")
			return 405;
		return 200;
	}
	else if (this->fileOpen(this->_pathTarget))		// Generic case
		return 200;
	else											// Error case
	{
		this->_pathTarget = "/error/404.html"; 				// path = locateError (TO DO)
		return 404;
	}
	return 200;
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
	if (_method != "GET" && _method != "POST" && _method != "DELETE")		// _method != _allowedMethods (Config File)
		code = 405;
	while (line[i] != ' ')
		this->_pathTarget.insert(_pathTarget.end(), line[i++]);
	i++;
	// If _pathTarget is invalid (Parsing target path), then invalid page
	if (code == 200)
		code = this->parsePath();
	while (line[i] != '\r')
		this->_protocol.insert(_protocol.end(), line[i++]);
	if (_protocol != "HTTP/1.1" && code == 200)
		code = 400;
	if (code != 200)
		_pathTarget = this->_errorPage[code];
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
		if (code != 200)
			break ;
		// Parse header
	}
	this->_responseCode = code;
}

//////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////// GETTER /////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

std::string Request::getMethod()
{
	return _method;
}

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


