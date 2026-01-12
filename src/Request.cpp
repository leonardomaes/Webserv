/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rda-cunh <rda-cunh@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 17:26:07 by lmaes             #+#    #+#             */
/*   Updated: 2026/01/10 15:54:36 by rda-cunh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Request.hpp"

Request::Request() : _method(""), _pathTarget(), _protocol(""), _firstLine(1), _responseCode(200), _isChunked(false)
{
	// printMsg("Constructor 1");
	_header["Host"] = "";
	_header["Connection"] = "";
	_header["Accept"] = "";
	_errorPage[400] = "/error/400.html";		// Config File
	_errorPage[401] = "/error/401.html";
	_errorPage[403] = "/error/403.html";
	_errorPage[404] = "/error/404.html";
	_errorPage[405] = "/error/405.html";
	_body = "";
	_root = "";
	_query = "";
}

Request::Request(const Request& obj)
{
	// printMsg("Constructor 2");
	_method = obj._method;
	_pathTarget = obj._pathTarget;
	_protocol = obj._protocol;
	_isChunked = obj._isChunked;
	_firstLine = obj._firstLine;
	_responseCode = obj._responseCode;
	_errorPage = obj._errorPage;
	_conf = obj._conf;
	_root = obj._root;
	_query = obj._query;
}

Request& Request::operator=(const Request &obj)
{
    if (this != &obj)
    {
        this->_method = obj._method;
        this->_pathTarget = obj._pathTarget;
        this->_query = obj._query;
        this->_protocol = obj._protocol;
        this->_root = obj._root;
        this->_body = obj._body;

        this->_queryContent = obj._queryContent;
        this->_header = obj._header;
        this->_bodyContent = obj._bodyContent;
        this->_errorPage = obj._errorPage;

        this->_isChunked = obj._isChunked;
        this->_firstLine = obj._firstLine;
        this->_responseCode = obj._responseCode;

        this->_conf = obj._conf;
    }
    return *this;
}

Request::~Request()
{
	
}

Request::Request(ServerConfig conf) : _method(""), _pathTarget(), _protocol(""), _firstLine(1),  _responseCode(200), _isChunked(false)
{
	// printMsg("Constructor 3");
	_header["Host"] = "";
	_header["Connection"] = "";
	_header["Accept"] = "";
	for (std::map<int, std::string>::iterator it = conf.error_pages.begin(); it != conf.error_pages.end(); it++)
	{
		size_t dot = it->second.find('.');
		// if (dot == std::string::npos)
		// 	_errorPage[it->first] = it->second;
		size_t slash = it->second.find('/') + 1;
		if (slash == std::string::npos)
			slash = 0;
		std::stringstream ss;
		ss << it->first;
		_errorPage[it->first] = it->second;
		_errorPage[it->first].replace(slash, dot - slash, ss.str());
	}
	_conf = conf;
	_body = "";
	_root = "";
	_query = "";
}



int Request::fileOpen(std::string target)
{
	std::string filename = this->_conf.root;	// Config file
	filename.append(target);
	std::cout << filename << std::endl;
	std::ifstream file(filename.c_str(), std::ios::in);
	if (!file.is_open())
		return 0;
	file.close();
	return 1;
}

//////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////// POST TEXT ////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////


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

std::map<std::string, std::string> Request::parseUrlEncodedBody()
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

		if (val == "")
			_responseCode = 400;
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


//////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////// POST IMAGE ///////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////


std::string Request::getMultipartBoundary()
{
	std::string content_type = _header["Content-Type"];
	size_t pos = content_type.find("boundary=");
	if (pos == std::string::npos)
		return "";

	return "--" + content_type.substr(pos + 9);
}

std::string Request::extractFilename(const std::string& headers)
{
	size_t pos = headers.find("filename=");
	if (pos == std::string::npos)
		return "";

	pos += 9;
	if (headers[pos] == '"')
		pos++;

	size_t end = headers.find_first_of("\"\r\n", pos);
	return headers.substr(pos, end - pos);
}

std::string Request::sanitizeFilename(const std::string& filename)
{
	std::string clean;

	for (size_t i = 0; i < filename.size(); i++)
	{
		if (isalnum(filename[i]) || filename[i] == '.' || filename[i] == '_')
			clean += filename[i];
	}

	if (clean.empty())
		clean = "upload.bin";

	return clean;
}

bool Request::writeBinaryFile(const std::string& path, const std::string& data)
{
	std::ofstream ofs(path.c_str(), std::ios::binary);
	if (!ofs.is_open())
		return false;

	ofs.write(data.data(), data.size());
	ofs.close();
	return true;
}


void Request::parseMultipartImage()
{
	std::string boundary = getMultipartBoundary();
	if (boundary.empty())
	{
		_responseCode = 400;
		return ;
	}
	size_t pos = _body.find(boundary);
	if (pos == std::string::npos)
	{
		_responseCode = 400;
		return ;
	}
	pos += boundary.length() + 2;

	size_t headers_end = _body.find("\r\n\r\n", pos);
	if (headers_end == std::string::npos)
	{
		_responseCode = 400;
		return ;
	}
	std::string part_headers = _body.substr(pos, headers_end - pos);
	std::string filename = extractFilename(part_headers);
	if (filename.empty())
	{
		_responseCode = 400;
		return ;
	}
	filename = sanitizeFilename(filename);
	size_t data_start = headers_end + 4;
	size_t data_end = _body.find(boundary, data_start);
	if (data_end == std::string::npos || data_end < 2)
	{
		_responseCode = 400;
		return;
	}

	data_end -= 2;
	std::string file_data = _body.substr(data_start, data_end - data_start);
	std::string upload_path = _root + _pathTarget + "/" + filename;

	if (!writeBinaryFile(upload_path, file_data))
	{
		_responseCode = 500;
		return;
	}

	_responseCode = 201;
}


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////// PARSING /////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////


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
	if (key == "Transfer-Encoding" && value == "chunked")
	{
		_isChunked = true;
		_responseCode = 411;
	}
	
}

int Request::validLocation(std::string filename)
{	// Check if the LOCATION exist
	size_t it_l = 0;
	size_t best = _conf.locations.size();
	size_t bestLen = 0;

	for (size_t i = 0; i < _conf.locations.size(); i++)
	{
		const std::string& loc = _conf.locations[i].path;

		if (loc == "/" || (filename.compare(0, loc.size(), loc) == 0 && (filename.size() == loc.size() || filename[loc.size()] == '/')))
		{		// may have problems here
			if (loc.size() > bestLen)
			{
				best = i;
				bestLen = loc.size();
			}
		}
		printMsg("Filename: " + _conf.locations[i].path);
	}
	printMsg("Filename: " + filename);
	it_l = best;
	bool allowed = false;
	if (best == _conf.locations.size()) // Case there is no location
	{
		if (this->_method == "GET" || this->_method == "POST" || this->_method == "DELETE")
			allowed = true;
		_root = _conf.root;
	}
	else
	{
		// Check in the location for the ALLOWED METHODS
		for (size_t i = 0; i < _conf.locations[it_l].allow_methods.size(); i++)
		{
			if (_conf.locations[it_l].allow_methods[i] == this->_method)
			{
				allowed = true;
				break;
			}
			printMsg("Location: " + _conf.locations[it_l].path);
			printMsg("Location: " + filename);
			printMsg("Method: " + _conf.locations[it_l].allow_methods[i]);
			printMsg("Method: " + this->_method);
		}
		printMsg("Location Root>" + _conf.locations[it_l].root + "<");
		if (_conf.locations[it_l].root != "")
			_root = _conf.locations[it_l].root;
		else
			_root = _conf.root;
	}
	if (_pathTarget == "/")
		_pathTarget = "/" + _conf.index;
	if (!allowed)
		return 405;  // 404
	if (this->_method == "POST")
		return 201;
	else
		return 200;
}

int Request::parsePath()
{
	if (this->_pathTarget == "/favicon.ico")	// Special case
	{
		if (this->_method != "GET")
			return 405;
		_root = _conf.root;
		return 200;
	}
	printMsg("Root:" + this->_root);
	int code = 200;
	if (this->_method == "POST")
		code = 201;
	code = validLocation(this->_pathTarget);
	if (code >= 400)
		return code;
	// DELETE special case (only checks existance)
	if (this->_method == "DELETE")
	{
		std::string fullPath = this->_root + this->_pathTarget;
		struct stat pathStat;
		if (stat(fullPath.c_str(), &pathStat) == 0)
		{
			printMsg("DELETE target exists: " + fullPath);
			return 200;
		}
		else
		{
			printMsg("DELETE target not found: " + fullPath);
			this->_pathTarget = "/error/404.html";
			return 404;
		}
	}
		std::cout << _pathTarget << std::endl;
	if (this->fileOpen(this->_pathTarget))		// Generic case (GET/POST)
		return code;
	else										// Error case
	{
		this->_pathTarget = "/error/404.html";
		return 404;
	}
	return code;
}

int Request::parseConfig()
{
	int code = 200;
	if (this->_method == "POST")
		code = 201;
	// If _pathTarget is invalid (Parsing target path), then invalid page
	if (code < 400)
		code = this->parsePath();
	// std::cout << _pathTarget << std::endl;
	return code;
}

void Request::parseBody(std::string &buffer, size_t header_end)
{
	if (header_end == std::string::npos)
		return;

	size_t body_start = header_end + 4;

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
	if (content_length > _conf.client_max_body_size)
	{
		_responseCode = 413;
		return ;
	}
	_body = buffer.substr(body_start, content_length);
	std::string content_type = _header["Content-Type"];

	if (content_type.find("multipart/form-data") != std::string::npos)
	{
		parseMultipartImage();
	}
	else if (content_type.find("application/x-www-form-urlencoded") != std::string::npos)
	{
		_bodyContent = parseUrlEncodedBody();

		// to allow the DELETE form to work
		// if this is a method override (DELETE) does not do like a file upload
		if (_bodyContent.find("_method") != _bodyContent.end())
			return;
		// if filenamane or content are missing it's an DELETE and not a file upload
		if (_bodyContent.find("filename") == _bodyContent.end() || _bodyContent.find("content") == _bodyContent.end())
			return;

		std::string filename = _root + _pathTarget + '/' + _bodyContent["filename"];	// Erro (Need to remove one slash bar from full path)
		printMsg("Filename(Response):" + filename);
		if (_bodyContent["filename"].find("..") != std::string::npos || _bodyContent["filename"].find("/") != std::string::npos)
		{
			_responseCode = 400;
			return ;
		}
		std::string content = _bodyContent["content"];
		std::ofstream file(filename.c_str(), std::ios::binary | std::ios::out);
		if (!file.is_open())
		{
			_responseCode = 400;
			return ;
		}
		file << content;
		file.close();
	}
	else
		_responseCode = 415;
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////// FUNCTIONS ///////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

void Request::parseTarget(const std::string& target)
{
	size_t qpos = target.find('?');
	if (qpos == std::string::npos)
	{
		_pathTarget = target;
		_query = "";
	}
	else
	{
		_query = target.substr(qpos+1);
		_pathTarget = target.substr(0, qpos);

		size_t eq = _query.find('=');
		if (eq != std::string::npos)
		{
			std::string key = decodeUrl(_query.substr(0, eq));
			std::string val = decodeUrl(_query.substr(eq + 1));
			_queryContent[key] = val;
			_pathTarget += '/' + _queryContent[key];
		}
	}
}

void Request::parseRequest(std::string buffer)
{
	std::string line;
	std::istringstream request(buffer);
	_responseCode = 200;
	size_t header_end = buffer.find("\r\n\r\n");
	if (header_end == std::string::npos)
		_responseCode = 400;
	while (std::getline(request, line))
	{
		if (_firstLine)
		{
			_responseCode = this->parseFirstLine(line);
			parseTarget(_pathTarget);
			if (_responseCode >= 400)
				break ;
			continue;
		}
		if (line == "\r" || line.empty() || _isChunked)
			break;
		parseHeader(line);
	}
	if (_responseCode < 400)
		_responseCode = parseConfig();
	if (_responseCode < 400 && _method == "POST")
		parseBody(buffer, header_end);
	printMsg("Root:" + this->_root);
	printMsg("Target:" + this->_pathTarget);
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

std::string Request::getRoot() const
{
	return _root;
}

std::string Request::getConnection() const
{
	return _header.at("Connection");
}

std::string Request::getBodyContent(std::string key) const
{
	return _bodyContent.at(key);
}

const std::string Request::getErrorPage(int error) const
{
	std::map<int, std::string>::const_iterator it = _errorPage.find(error);

	if (it == _errorPage.end())
	{
		std::stringstream ss;
		ss << "error/" << error << ".html";
		return ss.str();
	}

	return it->second;
}

std::string Request::getBody() const
{
	return _body;
}

const ServerConfig *Request::getConfig() const
{
	return &_conf;
}

bool Request::isMultipart() const
{
	std::string ct = _header.at("Content-Type");
	return ct.find("multipart/form-data") != std::string::npos;
}

bool Request::isChunked() const
{
	return _isChunked;
}

//////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////// SETTER /////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

void Request::setPathTarget(const std::string &path)
{
	_pathTarget = path;
}

void Request::setMethod(std::string method)
{
	_method = method;
}


