/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rda-cunh <rda-cunh@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 17:24:27 by lmaes             #+#    #+#             */
/*   Updated: 2026/01/01 15:23:11 by rda-cunh         ###   ########.fr       */
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

std::string Response::getContent(Request &obj)	// Add dynamic error based in http code (TO DO)
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
		// is a directory
		printMsg("Path is a directory: " + fullPath);

		// try to serve index file
		std::string indexPath = fullPath;
		if (!indexPath.empty() && indexPath[indexPath.length() - 1] != '/')
			indexPath += "/";
		indexPath += "index.html";
		
		struct stat indexStat;
		if (stat(indexPath.c_str(), &indexStat) == 0 && S_ISREG(indexStat.st_mode))
		{
			// index file exists -> update the path and send it
			printMsg("serving index file: " + indexPath);
			std::string newPath = obj.getPathTarget();
			if (newPath[newPath.length() - 1] != '/')
				newPath += "/";
			newPath += "index.html"; 
			obj.setPathTarget(newPath);
		}
		else
		{
			// if no index file, check if autoindex is enabled
			if (isAutoIndexEnabled(obj))
			{
				printMsg("Autoindex enabled, generating directory listing");
				handleDirectoryListing(obj, eventFD);
				return;
			}
			else
			{
				// if autoindex is disabled and no index file, send 403 error
				printMsg("Autoindex disabled and no index file - 403 Forbiden");
				handleERROR(obj, 403, eventFD);
				return;
			}
		}			

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
void Response::generateResponse(Request &obj, int epfd, int eventFD)		// TO DO	//RM: changed "Request obj" into "Request &obj"
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

	// normalize the path (remove slash for comparing)
	std::string normalizedPath = path;
	if (normalizedPath.length() > 1 && normalizedPath[normalizedPath.length() - 1] == '/')
		normalizedPath = normalizedPath.substr(0, normalizedPath.length() - 1);

	// match the request path with the correspondent location in config
	const std::vector<ServerConfig> &servers = obj.getConfig()->getServers();

	// CHECK THIS LATER WITH LEO: for now implementing the search for location on the first server, but ideally we need to match the search with the correct server (host and port)
	// matching the request path with the correct location in config
	if (servers.empty())
		return (false);
	
	const ServerConfig &server = servers[0]; // EVALUATE TO IMPROVE THIS

	// finding the correct location
	const LocationConfig* bestMatch = NULL;
	size_t longestMatch = 0;
	
	for (size_t i = 0; i < server.locations.size(); ++i)
	{
		const LocationConfig &loc = server.locations[i];

		// normalize location path
		std::string locPath = loc.path;
		if (locPath.length() > 1 && locPath[locPath.length() - 1] == '/')
			locPath = locPath.substr(0, locPath.length() - 1);

		// check if request path starts with location path
		// checking first the exact match and then the prefix match followed by '/'
		if (normalizedPath == locPath)
		{
			// exact match
			if (locPath.length() > longestMatch)
			{
				longestMatch = locPath.length();
				bestMatch = &loc;
			}
		}
		else if (normalizedPath.find(locPath) == 0)
		{
			// prefix match served after
			if (normalizedPath.length() > locPath.length() && 
			    normalizedPath[locPath.length()] == '/')
			{
				if (locPath.length() > longestMatch)
				{
					longestMatch = locPath.length();
					bestMatch = &loc;
				}
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
	
	std::string response = header + body;

	// sending the response
	send(eventFD, response.c_str(), response.size(), 0);

	printMsg("Directory list sent successfully");
}

std::string Response::generateDirectoryHTML(const std::string &dirPath, const std::string &uriPath)
{
	DIR* dir = opendir(dirPath.c_str());
	if (!dir)
	{
		std::cerr << "LOG:: " << RED << "Failed to open directory: " << dirPath << RESET << std::endl;
		return "";
	}

	// stores directory entries
	std::vector<std::string> directories;
	std::vector<std::string> files;

	struct dirent *entry;
	while ((entry = readdir(dir)) != NULL)
	{
		std::string name = entry->d_name;

		if (name == ".")	// skip current directory 
			continue;

		// Build full path for stat
		std::string fullEntryPath = dirPath;
		if (!dirPath.empty() && dirPath[dirPath.length() - 1] != '/')
			fullEntryPath += "/";
		fullEntryPath += name;

		struct stat entryStat;
		if (stat(fullEntryPath.c_str(), &entryStat) == 0)
		{
			if (S_ISDIR(entryStat.st_mode))
				directories.push_back(name);
			else
				files.push_back(name);
		}
	}
	closedir(dir);

	// sort entries alphabetically
	std::sort(directories.begin(), directories.end());
	std::sort(files.begin(), files.end());

	// ensure that uriPath ends with '/'
	std::string basePath = uriPath;
	if (!basePath.empty() && basePath[basePath.length() - 1] != '/')
		basePath += "/";

	// build HTML
	std::stringstream html;
	html << "<!DOCTYPE html>\n"
         << "<html>\n"
         << "<head>\n"
         << "    <meta charset=\"UTF-8\">\n"
         << "    <title>Index of " << uriPath << "</title>\n"
         << "    <style>\n"
         << "        body { font-family: Arial, sans-serif; margin: 40px; }\n"
         << "        h1 { color: #333; border-bottom: 2px solid #666; padding-bottom: 10px; }\n"
         << "        table { width: 100%; border-collapse: collapse; margin-top: 20px; }\n"
         << "        th { background-color: #f0f0f0; text-align: left; padding: 10px; border-bottom: 2px solid #666; }\n"
         << "        td { padding: 8px; border-bottom: 1px solid #ddd; }\n"
         << "        tr:hover { background-color: #f5f5f5; }\n"
         << "        a { color: #0066cc; text-decoration: none; }\n"
         << "        a:hover { text-decoration: underline; }\n"
         << "        .dir { font-weight: bold; }\n"
         << "        .icon { margin-right: 5px; }\n"
         << "    </style>\n"
         << "</head>\n"
         << "<body>\n"
         << "    <h1>Index of " << uriPath << "</h1>\n"
         << "    <table>\n"
         << "        <tr><th>Name</th><th>Type</th></tr>\n";

	// adding a parent directory link (if not root)
	if (uriPath != "/")
	{
		std::string parentPath = uriPath;
		size_t lastSlash = parentPath.find_last_of('/');
		if (lastSlash != std::string::npos && lastSlash > 0)
			parentPath = parentPath.substr(0, lastSlash);
		else
			parentPath = "/";

		html << "        <tr>\n"
             << "            <td><a href=\"" << parentPath << "\">📁 ../</a></td>\n"
             << "            <td>Directory</td>\n"
             << "        </tr>\n";
	}

	// add directories
	for (size_t i = 0; i < directories.size(); ++i)
	{
		html << "        <tr>\n"
             << "            <td class=\"dir\"><a href=\"" << basePath << directories[i] << "/\">📁 " 
             << directories[i] << "/</a></td>\n"
             << "            <td>Directory</td>\n"
             << "        </tr>\n";
	}

	// add files
	for (size_t i = 0; i < files.size(); ++i)
    {
        html << "        <tr>\n"
             << "            <td><a href=\"" << basePath << files[i] << "\">📄 " 
             << files[i] << "</a></td>\n"
             << "            <td>File</td>\n"
             << "        </tr>\n";
    }

	html << "    </table>\n"
		<< "    <hr>\n"
		<< "    <p><em>Webserv/1.0 Server</em></p>\n"
		<< "</body>\n"
		<< "</html>\n";

	return (html.str());	
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

