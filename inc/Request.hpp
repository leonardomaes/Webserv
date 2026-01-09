/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rda-cunh <rda-cunh@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 17:25:57 by lmaes             #+#    #+#             */
/*   Updated: 2026/01/09 14:23:18 by rda-cunh         ###   ########.fr       */
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
	std::string _protocol;
	std::string _root;
	std::map<std::string, std::string> _header;
	std::string _body;

	// Variables
	bool		_firstLine;
	int			_responseCode;
	std::map<int, std::string> _errorPage;

	// Objs
	ServerConfig _conf;

	// Functions
	int parseFirstLine(std::string line);
	int	parsePath();
	int validLocation(std::string filename);
	int fileOpen(std::string filename);
	void parseHeader(std::string line);
	int parseConfig();
	size_t getContentLength() const;
	void parseBody(std::string &buffer, size_t header_end);
	std::string decodeUrl(const std::string &str) const;
	void parseTarget(const std::string& target);
public:
	Request();
	Request(const Request& obj);
	// Request& operator=(const Request& obj);
	~Request();
	
	Request(ServerConfig conf);

// Functions
	void parseRequest(std::string buffer);
	std::map<std::string, std::string> parseUrlEncodedBody() const;

// Getters
	std::string getMethod() const;
	std::string getPathTarget() const;
	std::string getProtocol() const;
	std::string getRoot() const;
	std::string getConnection() const;
	std::string getBody() const;
	int			getCode() const;
	const std::string getErrorPage(int error) const;
	const ServerConfig *getConfig() const;	// RN: need to add this getter here to access the config info

// Setters
	void setPathTarget(const std::string &path);

// Exception
	class InvalidRequest : public std::exception { const char* what() const throw(); };
};
