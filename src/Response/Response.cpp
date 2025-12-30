/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rda-cunh <rda-cunh@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 17:24:27 by lmaes             #+#    #+#             */
/*   Updated: 2025/12/30 12:15:26 by rda-cunh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/Response/Response.hpp"

Response::Response()
{
	this->_root = "assets/html";  // Config File
	this->FillStatus();
	_handler["GET"] = &Response::handleGET;
	_handler["POST"] = &Response::handlePOST;
	_handler["DELETE"] = &Response::handleDELETE;
	
}

Response::Response(const Response& obj)
{
	_status = obj._status;
	_root = obj._root;
}

// Response& Response::operator=(const Response& obj)
// {
// 	if (this != &obj)
// 	{
// 		*this = obj;
// 	}
// 	return *this;
// }

Response::~Response()
{
}

void Response::FillStatus()
{
	// this->_status = {{100, "Continue"}, {101, "Switching Protocols"},
	// 		{102, "Processing"}, {103, "Early Hints"}, {200, "OK"}, {201, "Created"},
	// 		{202, "Accepted"}, {203, "Non-Authoritative Information"}, {204, "No Content"},
	// 		{205, "Reset Content"}, {206, "Partial Content"}, {207, "Multi-Status"},
	// 		{208, "Already Reported"}, {226, "IM Used"}, {300, "Multiple Choices"}, {301, "Moved Permanently"},
	// 		{302, "Found"}, {303, "See Other"}, {304, "Not Modified"}, {305, "Use Proxy"}, {306, "Unused"},
	// 		{307, "Temporary Redirect"}, {308, "Permanent Redirect"}, {400, "Bad Request"}, {401, "Unauthorized"},
	// 		{402, "Payment Required"}, {403, "Forbidden"}, {404, "Not Found"}, {405, "Method Not Allowed"},
	// 		{406, "Not Acceptable"}, {407, "Proxy Authentication Required"}, {408, "Request Timeout"}, {409, "Conflict"},
	// 		{410, "Gone"}, {411, "Length Required"}, {412, "Precondition Failed"}, {413, "Content Too Large"},
	// 		{414, "URI Too Long"}, {415, "Unsupported Media Type"}, {416, "Range Not Satisfiable"}, {417, "Expectation Failed"},
	// 		{418, "I'm a teapot"}, {421, "Misdirected Request"}, {422, "Unprocessable Content"}, {423, "Locked"},
	// 		{424, "Failed Dependency"}, {425, "Too Early"}, {426, "Upgrade Required"}, {428, "Precondition Required"},
	// 		{429, "Too Many Requests"}, {431, "Request Header Fields Too Large"}, {451, "Unavailable For Legal Reasons"},
	// 		{500, "Internal Server Error"}, {501, "Not Implemented"}, {502, "Bad Gateway"}, {503, "Service Unavailable"},
	// 		{504, "Gateway Timeout"}, {505, "HTTP Version Not Supported"}, {506, "Variant Also Negotiates"},
	// 		{507, "Insufficient Storage"}, {508, "Loop Detected"}, {510, "Not Extended"}, {511, "Network Authentication Required"}};

	_status[100] = "Continue";
	_status[101] = "Switching Protocols";
	_status[102] = "Processing";
	_status[103] = "Early Hints";

	_status[200] = "OK";
	_status[201] = "Created";
	_status[202] = "Accepted";
	_status[203] = "Non-Authoritative Information";
	_status[204] = "No Content";
	_status[205] = "Reset Content";
	_status[206] = "Partial Content";
	_status[207] = "Multi-Status";
	_status[208] = "Already Reported";
	_status[226] = "IM Used";

	_status[300] = "Multiple Choices";
	_status[301] = "Moved Permanently";
	_status[302] = "Found";
	_status[303] = "See Other";
	_status[304] = "Not Modified";
	_status[305] = "Use Proxy";
	_status[306] = "Unused";
	_status[307] = "Temporary Redirect";
	_status[308] = "Permanent Redirect";

	_status[400] = "Bad Request";
	_status[401] = "Unauthorized";
	_status[402] = "Payment Required";
	_status[403] = "Forbidden";
	_status[404] = "Not Found";
	_status[405] = "Method Not Allowed";
	_status[406] = "Not Acceptable";
	_status[407] = "Proxy Authentication Required";
	_status[408] = "Request Timeout";
	_status[409] = "Conflict";
	_status[410] = "Gone";
	_status[411] = "Length Required";
	_status[412] = "Precondition Failed";
	_status[413] = "Content Too Large";
	_status[414] = "URI Too Long";
	_status[415] = "Unsupported Media Type";
	_status[416] = "Range Not Satisfiable";
	_status[417] = "Expectation Failed";
	_status[418] = "I'm a teapot";
	_status[421] = "Misdirected Request";
	_status[422] = "Unprocessable Content";
	_status[423] = "Locked";
	_status[424] = "Failed Dependency";
	_status[425] = "Too Early";
	_status[426] = "Upgrade Required";
	_status[428] = "Precondition Required";
	_status[429] = "Too Many Requests";
	_status[431] = "Request Header Fields Too Large";
	_status[451] = "Unavailable For Legal Reasons";

	_status[500] = "Internal Server Error";
	_status[501] = "Not Implemented";
	_status[502] = "Bad Gateway";
	_status[503] = "Service Unavailable";
	_status[504] = "Gateway Timeout";
	_status[505] = "HTTP Version Not Supported";
	_status[506] = "Variant Also Negotiates";
	_status[507] = "Insufficient Storage";
	_status[508] = "Loop Detected";
	_status[510] = "Not Extended";
	_status[511] = "Network Authentication Required";
}

std::string Response::getContent(Request obj)	// Add dynamic error based in http code (TO DO)
{
	std::string result;
	std::string path = this->getRoot();
	path.append(obj.getPathTarget());
	printMsg(path + " (path)");
	std::ifstream file(path.c_str(), std::ios::in);
	if (!file.is_open())
	{
		std::cout << "LOG:: " << RED << "Couldn't open target file; (Response::getContent)\n" << RESET;
		return "";
	}
	
	char buffer[4096];

	while (file.read(buffer, sizeof(buffer)) || file.gcount() > 0)
		result.append(buffer, file.gcount());

	return result;
}

std::string Response::getStatus(Request obj)
{
	std::string status;
	std::stringstream ss;
	if (obj.getCode() != 200)
	{
		//std::cout << "-DBG::" << obj.getProtocol() << "(Protocol - 2)" << std::endl;	// DELETE
		ss << obj.getCode();
		status = obj.getProtocol() + " " + ss.str() + " " + _status[obj.getCode()] + "\r\n";
		return status;
	}
	return "HTTP/1.1 200 OK\r\n";
}

void Response::sendFavicon(Request obj, int eventFD)
{
	std::vector<char> data;
	std::string path = _root + obj.getPathTarget();
	std::ifstream file(path.c_str(), std::ios::in);
	if (!file.is_open())
		throw ("Invalid (Request::sendFavicon)");
	file.seekg(0, std::ios::end);
	std::streamsize size = file.tellg();
	file.seekg(0, std::ios::beg);
	data.resize(size);
	file.read(&data[0], size);
	std::stringstream ss;
	ss << data.size();
	std::string header = "HTTP/1.1 200 OK\r\n"
						"Content-Type: image/x-icon\r\n"
						"Content-Length: " + ss.str() + "\r\n"
						"Connection: close\r\n\r\n";
    send(eventFD, header.c_str(), header.size(), 0);
    send(eventFD, &data[0], data.size(), 0);
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////// MEMBER FUNCTIONS ////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

void Response::handleGET(Request& obj, int eventFD)
{
	if (obj.getPathTarget() == "/icon/favicon.ico")
	{
		sendFavicon(obj, eventFD);
		return ;
	}

	// DIRECTORY LISTING
	std::string fullPath = this->getRoot() + obj.getPathTarget();

	// check if the path is a directory
	struct stat pathStat;
	if (stat(fullPath.c_str(), &pathStat) == 0 && S_ISDIR(pathStat.st_mode))
	{
		if (isAyt)
	}

	
	
	std::string header = this->getStatus(obj);
	std::string body = this->getContent(obj);
	printMsg(header + " (header)");
	std::stringstream ss;
	ss << body.size();
	std::string response =	header +					// Make it dynamic (TO DO)
							"Content-Type: text/html; charset=UTF-8\r\n"
							"Content-Length: " + ss.str() + "\r\n\r\n" +
							body;
	printMsg("\n");
	send(eventFD, response.c_str(), response.size(), 0);
}

void Response::handlePOST(Request& obj, int eventFD)
{
	// Upload / CGI
	(void)eventFD;
	(void)obj;
}

void Response::handleDELETE(Request& obj, int eventFD)
{
	// Delete resource
	(void)eventFD;
	(void)obj;
}

void Response::handleERROR(Request& obj, int error, int eventFD)
{
	std::stringstream ss1;
	ss1 << error;
	std::string header = "HTTP/1.1 " + ss1.str() + " " + _status[error] + "\r\n";
	std::string path = getRoot();
	path.append(obj.getErrorPage(error));
	std::ifstream file(path.c_str(), std::ios::in);
	std::string body;
	if (!file.is_open())
	{
		std::cout << "LOG:: " << RED << "Couldn't open error file; (Response::handleERROR)\n" << RESET;
		body = "";
	}
	char buffer[4096];
	while (file.read(buffer, sizeof(buffer)) || file.gcount() > 0)
		body.append(buffer, file.gcount());
	std::stringstream ss2;
	ss2 << body.size();
	std::string response = header + 
							"Content-Type: text/html; charset=UTF-8\r\n"
							"Content-Length: " + ss2.str() + "\r\n\r\n" +
							body;
	send(eventFD, response.c_str(), response.size(), 0);
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////// FUNCTIONS ///////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

// Response starts here
void Response::generateResponse(Request obj, int epfd, int eventFD)		// TO DO
{
	std::stringstream dbg_ss;
	printMsg(obj.getPathTarget() + " (target)");
	dbg_ss << obj.getCode() << " (code)";
	printMsg(dbg_ss.str());
	// ******************************************************************
	try
	{
		std::map<std::string, MethodHandler>::iterator it;
		it = _handler.find(obj.getMethod());
		if (it == _handler.end())
			throw ResponseException("Method Not Allowed");
		MethodHandler handler = it->second;
		(this->*handler)(obj, eventFD);
	}
	catch(const std::exception& e)
	{
		handleERROR(obj, 405, eventFD);
		std::cerr << e.what() << '\n';
	}
	// *******************************************************************
	if (obj.getConnection() != "keep-alive")
	{
		// Delete event from epoll
		epoll_ctl(epfd, EPOLL_CTL_DEL, eventFD, NULL);
		// TO DO
		// Check line, if "Connection: keep-alive", we must not close it
		close(eventFD);
	}
	if (obj.getCode() >= 400)
		std::cout << "LOG:: " << RED << "> Sended Response (" << obj.getCode() << " - "
					<< this->_status[obj.getCode()] << ")" << RESET << std::endl;		// LOG
	else
		std::cout << "LOG:: " << GREEN << "> Sended Response (" << obj.getCode() << " - "
					<< this->_status[obj.getCode()] << ")" << RESET << std::endl;		// LOG
}

//////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////// DIR LISTING //////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

bool Response::isAutoIndexEnabled(Request &obj)
{
	// identifying the request path
	std::string path = obj.getPathTarget();

	// match the request path with the correspondent location in config
	const std::vector<ServerConfig> &servers = obj.getConfig()->getServers();

	// CHECK THIS LATER WITH LEO: for now implementing the search for location on the first server, but ideally we need to match the search with the correct server (host and port)
	// matching the request path with the correct location in config
	if (servers.empty())
		return (false);
	
	const ServerConfig &server = servers[0]; // TO IMPROVE THIS

	// finding the correct location
	const LocationConfig* bestMatch = NULL;
	size_t longestMatch = 0;
	
	for (size_t i = 0; i < server.locations.size(); ++i)
	{
		const LocationConfig &loc = server.locations[i];
		// check if request path starts with location path 
		if (path.find(loc.path) == 0)
		{
			if (loc.path.length() > longestMatch)
			{
				longestMatch = loc.path.length();
				bestMatch = &loc;
			}
		}
	}

	// if we found a matching location, we return its auto_index falue (1 or 0)
	if (bestMatch)
		return (bestMatch->auto_index); // returns 1 if auto_index is 'on'
	
	// as a default
	return (false);
}

void Response::handleDirectoryListing(Request &obj, int eventFD)
{
	std::string fullPath = this->getRoot() + obj.getPathTarget();
	std::string uriPath = obj.getPathTarget();
	
	printMsg("Generatinng directory listing for: " + fullPath);

	// generate the HTML body (helper function)
	std::string body = generateDirectoryHTML(fullPath, uriPath);
	if (body.empty())	//safeguard for failing to read directory
	{
		handleERROR(obj, 403, eventFD);
		return;
	}

	// building the HTTP response (header + body)
	std::stringstream ss;
	ss << body.size();

	std::string header = "HTTP/1.1 200 OK\r\n"
						 "Content-Type: text/html; charset=UTF-8\r\n"
						 "Content-Length: " + ss.str() + "\r\n"
						 "Connection: close\r\n\r\n";
	
	
						 

}

//////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////// GETTER /////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

std::string Response::getRoot()
{
	return this->_root;
}


//////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////// EXCEPTIONS ///////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

Response::ResponseException::ResponseException(const std::string& error)
{
	_errorMsg = "Error: " + error;
}

Response::ResponseException::~ResponseException() throw() {}


const char *Response::ResponseException::what() const throw() {
	return _errorMsg.c_str();
}

