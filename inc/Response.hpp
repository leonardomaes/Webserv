/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rda-cunh <rda-cunh@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 17:24:09 by lmaes             #+#    #+#             */
/*   Updated: 2026/01/10 03:38:31 by rda-cunh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Header.hpp"
#include "Request.hpp"
#include "Utils.hpp"
#include "CGI.hpp"

#include <dirent.h>
#include <sys/stat.h>
#include <sstream>
#include <algorithm>

class Response
{
private:
	// Member Functions
	typedef void (Response::*MethodHandler)(const Request&, int);
	// Member Functions
	void		handleGET(const Request& obj, int eventFD);
	void		handlePOST(const Request& obj, int eventFD);
	bool		isDELETEAllowed(const Request& obj);
	void		handleDELETE(const Request& obj, int eventFD);
	void		handleCGI(const Request &obj, const LocationConfig* loc, int eventFD);     // CGI feature
	void		handleRedirect(const Request& obj, int eventFD);
	const LocationConfig	*getLocationConfig(const Request& obj);    // location matching (handler for CGI, isAutoIndexEnabled, etc.)
	std::string	defaultErrorPage(int error);
	void		handleERROR(const Request& obj, int error, int eventFD);
	void		sendFavicon(const Request& obj, int eventFD);
	void		respond(std::string header, std::string body, int eventFD);

	std::map<int, std::string> _status;
	std::string _root;
	std::map<std::string, MethodHandler> _handler;

	// AutoIndex feature
	bool isAutoIndexEnabled(const Request &obj);
	void handleDirectoryListing(const Request &obj, int eventFD);
	std::string generateDirectoryHTML(const std::string &dirPath, const std::string &uriPath);

	// Fill
	void FillStatus();

	// Functions
	std::string getContent(std::string filename);
	std::string getStatus(const Request& obj);
public:
	Response();
	Response(const Response& obj);
	// Response& operator=(const Response& obj);
	~Response();
// Functions
	void generateResponse(const Request& obj, int epfd, int eventFD);
	


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

