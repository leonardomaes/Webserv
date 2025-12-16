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

Request::Request() : _method(""), _pathTarget(), _protocol(""), _firstLine(1), _responseCode(200)
{
	_head["Host"] = "";
	_head["Connection"] = "";
	_head["Accept"] = "";
	_errorPage[400] = "/error/400.html";		// Config File
	_errorPage[401] = "/error/401.html";
	_errorPage[403] = "/error/403.html";
	_errorPage[404] = "/error/404.html";
	_errorPage[405] = "/error/405.html";
}

Request::Request(const Request& obj)
{
	_method = obj._method;
	_pathTarget = obj._pathTarget;
	_protocol = obj._protocol;
	_firstLine = obj._firstLine;
	_responseCode = obj._responseCode;
	_errorPage = obj._errorPage;
}

// Request& Request::operator=(const Request& obj)
// {
// 	if (this != &obj)
// 	{
// 		*this = obj;
// 	}
// 	return *this;	
// }

Request::~Request()
{
}

Request::Request(Config *conf) : _method(""), _pathTarget(), _protocol(""), _firstLine(1), _responseCode(200)
{
	_head["Host"] = "";
	_head["Connection"] = "";
	_head["Accept"] = "";
	_errorPage[400] = "/error/400.html";		// Config File
	_errorPage[401] = "/error/401.html";
	_errorPage[403] = "/error/403.html";
	_errorPage[404] = "/error/404.html";
	_errorPage[405] = "/error/405.html";
	_conf = conf;
	// (void)conf;
	// this->_conf = conf;
}

int Request::fileOpen(std::string target)
{
	std::string filename = "assets/html";	// Config file
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
		this->_pathTarget = "/error/404.html";
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
	// If _method is invalid , then Invalid Method
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

void Request::parseHeader(std::string line)
{
	if (line.empty() || line == "\r\n")
		return;
	size_t pos = line.find(":");
	if (pos == std::string::npos)
		return;
	std::string key = line.substr(0, pos);
	std::string value = line.substr(pos + 1);
	while (!value.empty() && (value[0] == ' ' || value[0] == '\t'))
		value.erase(0, 1);
	if (!value.empty() && value[value.size() - 1] == '\r')
		value.erase(value.size() - 1);
	if (_head[key] == "")
		_head[key] = value;
}

int Request::parseConfig()		// Issue #3 github	// Missing Config file to make
{

	return 200;
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
		{
			code = this->parseFirstLine(line);
			if (code != 200)
				break ;
			continue;
		}
		parseHeader(line);
	}
	if (code == 200)
		code = parseConfig();
	this->_responseCode = code;
}

//////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////// GETTER /////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

std::string Request::getMethod()
{
	return _method;
}

std::string Request::getPathTarget()
{
	return _pathTarget;
}

std::string Request::getProtocol()
{
	return _protocol;
}

int Request::getCode()
{
	return _responseCode;
}

std::string Request::getConnection()
{
	return _head["Connection"];
}
