/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rda-cunh <rda-cunh@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 17:25:57 by lmaes             #+#    #+#             */
/*   Updated: 2026/01/10 03:53:14 by rda-cunh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Header.hpp"
#include "Config.hpp"
#include "Utils.hpp"
#include <sys/stat.h>

class Request
{
private:
	// Request info
	std::string _method;
	std::string _pathTarget;
	std::string _query;
	std::map<std::string, std::string> _queryContent;
	std::string _protocol;
	std::string _root;
	std::map<std::string, std::string> _header;
	std::string _body;
	std::map<std::string, std::string> _bodyContent;

	// Variables
	bool		_firstLine;
	int			_responseCode;
	bool		_isChunked;
	std::map<int, std::string> _errorPage;

	// Objs
	ServerConfig _conf;

	// Functions
	int			parseFirstLine(std::string line);
	int			parsePath();
	int			validLocation(std::string filename);
	int			fileOpen(std::string filename);
	void		parseHeader(std::string line);
	int			parseConfig();
	size_t 		getContentLength() const;
	void		parseBody(std::string &buffer, size_t header_end);
	std::string	decodeUrl(const std::string &str) const;
	void		parseTarget(const std::string& target);
	std::map<std::string, std::string> parseUrlEncodedBody();

	void		parseMultipartImage();
	std::string getMultipartBoundary();
	std::string extractFilename(const std::string& headers);
	std::string sanitizeFilename(const std::string& filename);
	bool		writeBinaryFile(const std::string& path, const std::string& data);
public:
	Request();
	Request(const Request& obj);
	Request &operator=(const Request& obj);
	~Request();
	
	Request(ServerConfig conf);

// Functions
	void parseRequest(std::string buffer);

// Getters
	std::string getMethod() const;
	std::string getPathTarget() const;
	std::string getProtocol() const;
	std::string getRoot() const;
	std::string getConnection() const;
	std::string getBody() const;
	int			getCode() const;
	std::string getBodyContent(std::string key) const;
	const std::string getErrorPage(int error) const;
	const ServerConfig *getConfig() const;	// RN: need to add this getter here to access the config info
	bool isMultipart() const;
	bool isChunked() const;

// Setters
	void setPathTarget(const std::string &path);
	void setMethod(std::string method);

// Exception
	class InvalidRequest : public std::exception { const char* what() const throw(); };
};
