/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rda-cunh <rda-cunh@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 17:24:09 by lmaes             #+#    #+#             */
/*   Updated: 2026/01/01 14:42:54 by rda-cunh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../Webserv.hpp"
#include "../Request.hpp"
#include "../Utils.hpp"

#include <dirent.h>
#include <sys/stat.h>
#include <sstream>
#include <algorithm>

class Response
{
private:
	// Member Functions
	typedef void (Response::*MethodHandler)(Request&, int);
	// Member Functions
	void handleGET(Request &obj, int eventFD);
	void handlePOST(Request &obj, int eventFD);
	void handleDELETE(Request &obj, int eventFD);
	void handleERROR(Request& obj, int error, int eventFD);
	void sendFavicon(Request obj, int eventFD);

	std::map<int, std::string> _status;
	std::string _root;
	std::map<std::string, MethodHandler> _handler;

	// AutoIndex feature
	bool isAutoIndexEnabled(Request &obj);
	void handleDirectoryListing(Request &obj, int eventFD);
	std::string generateDirectoryHTML(const std::string &dirPath, const std::string &uriPath);

	// Fill
	void FillStatus();

	// Functions
	std::string getContent(Request &obj);
	std::string getStatus(Request obj);
public:
	Response();
	Response(const Response& obj);
	// Response& operator=(const Response& obj);
	~Response();
// Functions
	void generateResponse(Request &obj, int epfd, int eventFD);


// Getters
	std::string getRoot();

// Exception
	class ResponseException : public std::exception {
		private:
			std::string _errorMsg;
		public:
			ResponseException(const std::string& error);
			~ResponseException() throw();
			virtual const char* what() const throw();
	};
};