/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rda-cunh <rda-cunh@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/11 20:21:47 by rda-cunh          #+#    #+#             */
/*   Updated: 2026/01/11 23:40:11 by rda-cunh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CGI.hpp"

////////////////////////////////////////////////////////////////////////////////
///////////////////////////////// CONSTRUCTORS /////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

CGI::CGI(const std::string &scriptPath, const std::string &interpreterPath) :
    _scriptPath(scriptPath),
    _interepreterPath(interpreterPath)
{ }

CGI::~CGI() {}


////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////// FUNCTIONS //////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

void CGI::initializeEnv(const Request &request)
{
    _env["REQUEST_METHOD"] = request.getMethod();
    _env["QUERY_STRING"] = request.getQuery();                      // Does not exits in request class
    _env["CONTENT_LENGTH"] = request.getHeader("Content-Length");   // Does not exits in request class
    _env["CONTENT_TYPE"] = request.getHeader("Content-Type");       // Does not exits in request class
    _env["SCRIPT_NAME"] = _scriptPath;
    _env["SCRIPT_FILENAME"] = _scriptPath;
    _env["PATH_INFO"] = request.getPathTarget();
    _env["SERVER_PROTOCOL"] = "HTTP/1.1";
}

char ** CGI::getEnvAsArray() const
{
    // TO DO
}

std::string CGI::execute(const Request& request)
{
    initializeEnv(request);
    // TO DO
}