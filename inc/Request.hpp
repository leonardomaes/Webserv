/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rda-cunh <rda-cunh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 17:25:57 by lmaes             #+#    #+#             */
/*   Updated: 2026/01/19 16:15:45 by rda-cunh         ###   ########.fr       */
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
		std::string		_method;
		std::string		_pathTarget;
		std::string		_originalPath;
		std::string		_query;
		std::map<std::string, std::string> _queryContent;
		std::string		_protocol;
		std::string		_root;
		std::string		_matchedLocation;
		std::string		_locationIndex;
		std::map<std::string, std::string> _header;
		std::string		_body;
		std::map<std::string, std::string> _bodyContent;
		std::string		_redirURL;

		// Variables
		bool		_firstLine;
		int			_responseCode;
		bool		_isChunked;
		bool		_isBinary;
		bool		_isRedirect;
		std::map<int, std::string> _errorPage;

		// Objs
		ServerConfig	_conf;

		// ...
		int				fileOpen(std::string filename);
		
		// PARSING
		int				parseFirstLine(std::string line);
		void			parseTarget(const std::string& target);
		void			parseHeader(std::string line);
		std::string		buildFilesystemPath(bool hasRoot) const;
		int				validLocation(std::string filename);
		int				parsePath();
		void			parseBody(std::string &buffer, size_t header_end);

		// POST FILE
		bool			writeBinaryFile(const std::string& path, const std::string& data);
		std::string		extractFilename(const std::string& headers);
		std::string		sanitizeFilename(const std::string& filename);
		size_t 			getContentLength() const;
		std::string		decodeUrl(const std::string &str) const;
		std::string 	getMultipartBoundary();
		std::map<std::string, std::string> parseUrlEncodedBody();
		void			postMultipartFile();
		void			postFormFile();
		bool 			targetHasFilename() const;
		std::string		getFilenameFromTarget() const;
		std::string		getUploadDirectory() const;
		std::string		getUploadFilename() const;
		std::string 	generateFilename();
		void			postBinaryFile();
	
	public:
		Request();
		Request(const Request& obj);
		Request &operator=(const Request& obj);
		~Request();
		
		Request(ServerConfig conf);

		// Functions
		void parseRequest(std::string buffer);

		// Getters
		std::string			getMethod() const;
		std::string			getPathTarget() const;
		std::string			getOriginalPath() const;
		std::string			getQuery() const;
		std::string			getProtocol() const;
		std::string			getRoot() const;
		std::string			getConnection() const;
		std::string			getRedir() const;
		std::string			getBody() const;
		int					getCode() const;
		std::string			getHeaderContent(std::string key) const;
		std::string			getBodyContent(std::string key) const;
		const std::string	getErrorPage(int error) const;
		const ServerConfig	*getConfig() const;
		bool isMultipart() const;
		bool isChunked() const;
		bool isImage() const;
		bool isRedir() const;

		// Setters
		void setPathTarget(const std::string &path);
		void setMethod(std::string method);

		// Exception
		class InvalidRequest : public std::exception { const char* what() const throw(); };
	};
