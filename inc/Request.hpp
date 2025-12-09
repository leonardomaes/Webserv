/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmaes <lmaes@student.42porto.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 17:25:57 by lmaes             #+#    #+#             */
/*   Updated: 2025/10/27 17:25:58 by lmaes            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Webserv.hpp"

class Request
{
private:
	std::string _method;		// !!
	std::string _pathTarget;	// !!
	std::string _protocol;
	std::string _host;			// !!
	std::string _connection;
	std::string _accept;
	bool		_firstLine;
	int			_responseCode;
	int parseFirstLine(std::string line);
	int	parsePath();
	int fileOpen(std::string filename);
	std::map<int, std::string> _errorPage;
public:
	Request();
	// Request(std::string buffer);
	~Request();

// Functions
	void parseRequest(std::string buffer);
// Getters
	std::string getMethod();
	std::string getConnection();
	std::string getPathTarget();
	std::string getHost();
	std::string getProtocol();
	int			getCode();
// Exception
	class InvalidRequest : public std::exception { const char* what() const throw(); };
};
