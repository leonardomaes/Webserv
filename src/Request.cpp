/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rda-cunh <rda-cunh@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 17:26:07 by lmaes             #+#    #+#             */
/*   Updated: 2026/01/01 15:03:54 by rda-cunh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Request.hpp"

Request::Request() : _method(""), _pathTarget(), _protocol(""), _firstLine(1), _responseCode(200)
{
	_header["Host"] = "";
	_header["Connection"] = "";
	_header["Accept"] = "";
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
	_conf = obj._conf;
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

Request::Request(ServerConfig conf) : _method(""), _pathTarget(), _protocol(""), _firstLine(1), _responseCode(200)
{
	_header["Host"] = "";
	_header["Connection"] = "";
	_header["Accept"] = "";
	_errorPage[400] = "/error/400.html";		// Config File
	_errorPage[401] = "/error/401.html";
	_errorPage[403] = "/error/403.html";
	_errorPage[404] = "/error/404.html";
	_errorPage[405] = "/error/405.html";
	_conf = conf;
	_body = "";
}

int Request::fileOpen(std::string target)
{
	std::string filename = this->_conf.root;	// Config file
	filename.append(target);
	std::ifstream file(filename.c_str(), std::ios::in);
	if (!file.is_open())
		return 0;
	file.close();
	return 1;
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
	if (_method != "GET" && _method != "POST" && _method != "DELETE")	// It is already in Response::generateResponse
		code = 405;
	if (_method == "POST")
		code = 201;
	while (line[i] != ' ')
		this->_pathTarget.insert(_pathTarget.end(), line[i++]);
	i++;
	while (line[i] != '\r')
		this->_protocol.insert(_protocol.end(), line[i++]);
	if (_protocol != "HTTP/1.1" && code < 400)
		code = 400;
	if (code >= 400)
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
	if (_header[key] == "")
		_header[key] = value;
}

int Request::validLocation(std::string filename)
{	// Check if the location exist
	size_t it_l = 0;
	size_t best = _conf.locations.size();
	size_t bestLen = 0;

	for (size_t i = 0; i < _conf.locations.size(); i++)
	{
		const std::string& loc = _conf.locations[i].path;

		if (filename.compare(0, loc.size(), loc) == 0)
		{
			if (loc.size() > bestLen)
			{
				best = i;
				bestLen = loc.size();
			}
		}
	}
	if (best == _conf.locations.size())
		return 404;

	it_l = best;

	// Check in the location for the allowed methods
	bool allowed = false;
	for (size_t i = 0; i < _conf.locations[it_l].allow_methods.size(); i++)
	{
		if (_conf.locations[it_l].allow_methods[i] == this->_method)
		{
			allowed = true;
			break;
		}
	}
	if (!allowed)
		return 405;
	if (this->_method == "POST")
		return 201;
	else
		return 200;
}

int Request::parsePath()		// TO DO   (Config File)
{
	if (this->_pathTarget == "/favicon.ico")	// Special case
	{
		if (this->_method != "GET")
			return 405;
		return 200;
	}
	int code = 200;
	if (this->_method == "POST")
		code = 201;
	code = validLocation(this->_pathTarget);
	if (code >= 400)
		return code;
	if (this->fileOpen(this->_pathTarget))		// Generic case
		return code;
	else											// Error case
	{
		this->_pathTarget = "/error/404.html";
		return 404;
	}
	return code;
}

int Request::parseConfig()		// Missing Config file to make
{
	int code = 200;
	if (this->_method == "POST")
		code = 201;
	// If _pathTarget is invalid (Parsing target path), then invalid page
	if (code < 400)
		code = this->parsePath();
	if (code < 400)
	{
		if (this->_pathTarget == "/")
			_pathTarget = _conf.index;
		else if (this->_pathTarget == "/favicon.ico")
			_pathTarget = "/icon/favicon.ico";			// Changes to icon dir
	}
	// std::cout << _pathTarget << std::endl;
	return code;
}

size_t Request::getContentLength() const
{
	std::map<std::string, std::string>::const_iterator it;

	it = _header.find("Content-Length");
	if (it == _header.end())
		return 0;

	return static_cast<size_t>(std::atoi(it->second.c_str()));
}

std::string Request::decodeUrl(const std::string &str) const
{
	std::string out;

	for (size_t i = 0; i < str.size(); i++)
	{
		if (str[i] == '+')
			out += ' ';
		else if (str[i] == '%' && i + 2 < str.size())
		{
			char hex[3] = { str[i+1], str[i+2], 0 };
			out += static_cast<char>(std::strtol(hex, 0, 16));
			i += 2;
		}
		else
			out += str[i];
	}
	return out;
}

void Request::parseBody(std::string &buffer, size_t header_end)
{
	if (header_end == std::string::npos)
		return;

	size_t body_start = header_end + 4; // \r\n\r\n

	if (body_start >= buffer.size())
		return;

	size_t content_length = getContentLength();
	if (content_length == 0)
		return;

	if (body_start + content_length > buffer.size())
	{
		_responseCode = 400;
		return;
	}

	_body = buffer.substr(body_start, content_length);
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////// FUNCTIONS ///////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

std::map<std::string, std::string> Request::parseUrlEncodedBody() const
{
	std::map<std::string, std::string> result;

	std::string body = _body;
	size_t pos;

	while ((pos = body.find('&')) != std::string::npos)
	{
		std::string pair = body.substr(0, pos);
		body.erase(0, pos + 1);

		size_t eq = pair.find('=');
		if (eq == std::string::npos)
			continue;

		std::string key = decodeUrl(pair.substr(0, eq));
		std::string val = decodeUrl(pair.substr(eq + 1));

		result[key] = val;
	}

	size_t eq = body.find('=');
	if (eq != std::string::npos)
	{
		std::string key = decodeUrl(body.substr(0, eq));
		std::string val = decodeUrl(body.substr(eq + 1));
		result[key] = val;
	}
	return result;
}

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
			if (code >= 400)
				break ;
			continue;
		}
		if (line == "\r" || line.empty())
			break;
		parseHeader(line);
	}
	if (code < 400)
		parseBody(buffer, header_end);
	if (code < 400)
		code = parseConfig();
	this->_responseCode = code;
}

//////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////// GETTER /////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

std::string Request::getMethod() const
{
	return _method;
}

std::string Request::getPathTarget() const
{
	return _pathTarget;
}

std::string Request::getProtocol() const
{
	return _protocol;
}

int Request::getCode() const
{
	return _responseCode;
}

std::string Request::getConnection() const
{
	return _header.at("Connection");
}

const std::string Request::getErrorPage(int error) const
{
    std::map<int, std::string>::const_iterator it =
        _errorPage.find(error);

    if (it == _errorPage.end())
        return "";

    return it->second;
}

std::string Request::getBody() const
{
	return _body;
}

Config *Request::getConfig()
{
	return _conf;
}

//////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////// SETTER /////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

void Request::setPathTarget(const std::string &path)
{
    _pathTarget = path;
}
