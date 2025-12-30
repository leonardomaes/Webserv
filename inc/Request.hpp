/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rda-cunh <rda-cunh@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 17:25:57 by lmaes             #+#    #+#             */
/*   Updated: 2025/12/30 00:02:49 by rda-cunh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Webserv.hpp"
#include "Config.hpp"
#include "Utils.hpp"

class Request
{
private:
	// Request info
	std::string _method;		// !!
	std::string _pathTarget;	// !!
	std::string _protocol;
	std::map<std::string, std::string> _head;

	// Variables
	bool		_firstLine;
	int			_responseCode;
	std::map<int, std::string> _errorPage;

	// Objs
	Config *_conf;

	// Functions
	int parseFirstLine(std::string line);
	int	parsePath();
	int fileOpen(std::string filename);
	void parseHeader(std::string line);
	int parseConfig();
public:
	Request();
	Request(const Request& obj);
	// Request& operator=(const Request& obj);
	~Request();
	
	Request(Config *conf);

// Functions
	void parseRequest(std::string buffer);

// Getters
	std::string getMethod();
	std::string getPathTarget();
	std::string getProtocol();
	std::string getConnection();
	int			getCode();
	const std::string getErrorPage(int error);
	Config *getConfig();	// RN: need to add this getter here to access the config info

// Exception
	class InvalidRequest : public std::exception { const char* what() const throw(); };
};
