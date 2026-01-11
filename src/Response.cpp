/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rda-cunh <rda-cunh@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 17:24:27 by lmaes             #+#    #+#             */
/*   Updated: 2026/01/10 17:10:26 by rda-cunh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Response.hpp"

Response::Response()
{
	this->_root = "assets/html/";  // Config File
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

std::string Response::defaultErrorPage(int error)
{
	std::stringstream ss;

	ss << "<!DOCTYPE html>\n";
	ss << "<html>\n";
	ss << "<head>\n";
	ss << "<meta charset=\"UTF-8\">\n";
	ss << "<title>" << error << " " << _status[error] << "</title>\n";
	ss << "<style>\n";
	ss << "body { font-family: Arial; background-color: #f4f4f4; text-align: center; padding-top: 10%; }\n";
	ss << "h1 { font-size: 48px; }\n";
	ss << "p { font-size: 18px; }\n";
	ss << "</style>\n";
	ss << "</head>\n";
	ss << "<body>\n";
	ss << "<h1>" << error << " " << _status[error] << "</h1>\n";
	ss << "<p>The server encountered an error while processing your request.</p>\n";
	ss << "<br><br>";
	ss << "<table width=\"60%\" align=\"center\">";
	ss << "<tr>";
	ss << "<td align=\"center\">";
	ss << "<a href=\"/index.html\">";
	ss << "<input type=\"button\" value=\"Return to Home Page\">";
	ss << "</a>";
	ss << "</td>";
	ss << "</tr>";
	ss << "</table>";
	ss << "</body>\n";
	ss << "</html>\n";

	return ss.str();
}

std::string Response::getContent(std::string filename)	// Add dynamic error based in http code (TO DO)
{
	std::string result;
	std::string path = this->getRoot() + filename;
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

std::string Response::getStatus(const Request& obj)
{
	std::string status;
	std::stringstream ss;
	if (obj.getCode() != 200)
	{
		ss << obj.getCode();
		status = obj.getProtocol() + " " + ss.str() + " " + _status[obj.getCode()] + "\r\n";
		return status;
	}
	return "HTTP/1.1 200 OK\r\n";
}

void Response::sendFavicon(const Request& obj, int eventFD)
{
	std::vector<char> data;
	std::string path = getRoot() + obj.getPathTarget();
	printMsg("Favicon(path):" + path);
	std::ifstream file(path.c_str(), std::ios::in);
	if (!file.is_open())
		throw ResponseException("Invalid (Request::sendFavicon)");
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

void Response::respond(std::string header, std::string body, int eventFD)
{
	std::stringstream ss;
	ss << body.size();
	std::string response =	header +
							"Content-Type: text/html; charset=UTF-8\r\n"
							"Content-Length: " + ss.str() + "\r\n\r\n" +
							body;
	printMsg("\n");
	send(eventFD, response.c_str(), response.size(), 0);
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////// MEMBER FUNCTIONS ////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

void Response::handleGET(const Request& obj, int eventFD)
{
	if (obj.getCode() >= 400)
	{
		handleERROR(obj, obj.getCode(), eventFD);
		return ;
	}
	if (obj.getPathTarget() == "/favicon.ico")
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
			fullPath = indexPath;
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
	std::string body = this->getContent(obj.getPathTarget());
	printMsg(header + " (header)");
	respond(header, body, eventFD);
}

void Response::handlePOST(const Request& obj, int eventFD)
{
	if (obj.getCode() >= 400)
	{
		handleERROR(obj, obj.getCode(), eventFD);
		return ;
	}

	std::string body = obj.getBody();

    // check if this is a DELETE request masked as a POST (to allow delete.html to work)
    if (body.find("_method=DELETE") != std::string::npos)
    {
        printMsg("Method override detected: POST -> DELETE");

        // extract the filename from the "id" field
        std::string filename = getFormValue(body, "id");
        if (filename.empty())
        {
            handleERROR(obj, 400, eventFD); // if no ID provided return an error
            return;
        }

        // create a temp Request object (Copy of the original)
        Request tempReq = obj; 

        // modify the request to look like a DELETE request (adapt path and set method)
        std::string currentPath = obj.getPathTarget();
        if (currentPath[currentPath.length() - 1] != '/')
            currentPath += "/";
        tempReq.setPathTarget(currentPath + filename);
        tempReq.setMethod("DELETE");

        // forward to the request into the DELETE handler
        handleDELETE(tempReq, eventFD);
        return;
    }

	std::string header = this->getStatus(obj);					// Make it dynamic (TO DO)
	std::string responseBody;
	if (obj.isMultipart())
		responseBody = getContent("/upload_success.html");
	else	
		responseBody = getContent("/post_success.html");
	// printMsg(header);
	// printMsg(body);
	respond(header, responseBody, eventFD);
}

bool Response::isDELETEAllowed(const Request& obj)
{
	const ServerConfig *server = obj.getConfig();
	if (!server)
		return (false);

	std::string path = obj.getPathTarget();

	// finding marching locations (as implemented in AutoIndex)
	const LocationConfig* bestMatch = NULL;
	size_t longestMatch = 0;

	for (size_t i = 0; i < server->locations.size(); ++i)
	{
		const LocationConfig &loc = server->locations[i];
		std::string locPath = loc.path;

		if (path.find(locPath) == 0)
		{
			if (locPath.length() > longestMatch)
			{
				longestMatch = locPath.length();
				bestMatch = &loc;
			}
		}
	}

	if (!bestMatch)			// if no location was found
		return (false);

	//Check if DELETE is a method allowed
	for (size_t i = 0; i < bestMatch->allow_methods.size(); ++i)
	{
		if (bestMatch->allow_methods[i] == "DELETE")
			return (true);
	}

	return (false);
}

void Response::handleDELETE(const Request& obj, int eventFD)
{
	// checking for request errors
	if (obj.getCode() >= 400)
	{
		handleERROR(obj, obj.getCode(), eventFD);
		return;
	}

	// check if DELETE is an allowed method for this location
	if (!isDELETEAllowed(obj))
	{
		printMsg("DELETE method is not allowed at this location");
		handleERROR(obj, 405, eventFD);
		return;
	}

	// build full path (root + target path)
	std::string fullPath = this->getRoot() + obj.getPathTarget();
	printMsg("Received DELETE request for: " + fullPath);

	// avoid path traversal (security issue)
	if (obj.getPathTarget().find("..") != std::string::npos)
	{
		printMsg("Path traversal attempt");
		handleERROR(obj, 403, eventFD);
		return;
	}

	// check if resource exists
	struct stat pathStat;
	if (stat(fullPath.c_str(), &pathStat) != 0)
	{
		printMsg("Resource not found: " + fullPath);
		handleERROR(obj, 404, eventFD);
		return;
	}

	// check if it is a directory (folders cannot be deleted)
	if (S_ISDIR(pathStat.st_mode))
	{
		printMsg("Cannot delete a folder: " + fullPath);
		handleERROR(obj, 403, eventFD);
		return;		
	}

	// check file permissions (write)
	if (access(fullPath.c_str(), W_OK) != 0)
	{
		printMsg("Permission denied for: " + fullPath);
		handleERROR(obj, 403, eventFD);
		return;
	}

	// delete file, if it fails return error
	if (unlink(fullPath.c_str()) != 0)
	{
		printMsg("File delition failed for: " + fullPath);
		handleERROR(obj, 500, eventFD);
		return;
	}

	// print a log message for sucess
	printMsg("File sucessfull deleted: " + fullPath);

	// return a response (200 OK header and not 204 to allow the use of the delete.html)
	std::string header = "HTTP/1.1 200 OK\r\n";
	std::string body = getContent("/delete_success.html");  // this allow the redirection into delete_sucess.html
	respond(header, body, eventFD);
}

void Response::handleERROR(const Request& obj, int error, int eventFD)
{
	std::stringstream ss1;
	ss1 << error;

	std::string header = "HTTP/1.1 " + ss1.str() + " " + _status[error] + "\r\n";

	std::string body;
	std::string path = getRoot();
	path.append(obj.getErrorPage(error));

	std::ifstream file(path.c_str(), std::ios::in);
	if (file.is_open())
	{
		char buffer[4096];
		while (file.read(buffer, sizeof(buffer)) || file.gcount() > 0)
			body.append(buffer, file.gcount());
		file.close();
	}
	else
	{
		std::cout << "LOG:: " << RED << "Couldn't open error file, generating default error page (Response::handleERROR)\n" << RESET;
		body = defaultErrorPage(error);
	}

	std::stringstream ss2;
	ss2 << body.size();

	std::string response;
	response.reserve(header.size() + body.size() + 128);
	response = header +
			   "Content-Type: text/html; charset=UTF-8\r\n"
			   "Content-Length: " + ss2.str() + "\r\n"
			   "Connection: close\r\n"
			   "\r\n" +
			   body;

	send(eventFD, response.c_str(), response.size(), 0);
}

// Response starts here
void Response::generateResponse(const Request& obj, int epfd, int eventFD)
{
	this->_root = obj.getRoot();
	// printMsg("Body(test):" + obj.getBody() + "<");
	std::stringstream dbg_ss;
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
		std::cout << RED << "### " << obj.getConfig()->listen << " ###" << std::endl << "> Sended Response (" << obj.getCode() << " - "
					<< this->_status[obj.getCode()] << ")" << RESET << std::endl;		// LOG
	else
		std::cout << GREEN  << "### " <<  obj.getConfig()->listen << " ###" << std::endl <<  "> Sended Response (" << obj.getCode() << " - "
					<< this->_status[obj.getCode()] << ")" << RESET << std::endl;		// LOG
}

//////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////// DIR LISTING //////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

bool Response::isAutoIndexEnabled(const Request &obj)
{
	// identifying the request path
	std::string path = obj.getPathTarget();

	// getting the specific server config 
	const ServerConfig *server = obj.getConfig();
	if (!server)
		return (false);
	
	// normalize the path (remove slash for comparing)
	std::string normalizedPath = path;
	if (normalizedPath.length() > 1 && normalizedPath[normalizedPath.length() - 1] == '/')
		normalizedPath = normalizedPath.substr(0, normalizedPath.length() - 1);

	// finding the correct location on the server
	const LocationConfig* bestMatch = NULL;
	size_t longestMatch = 0;
	
	for (size_t i = 0; i < server->locations.size(); ++i)
	{
		const LocationConfig &loc = server->locations[i];

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

void Response::handleDirectoryListing(const Request &obj, int eventFD)
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

		if (name == "." || name == "..") 	// skip current and above directories 
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

	// ensure that uriPath ends with '/'		auto_index off;
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

