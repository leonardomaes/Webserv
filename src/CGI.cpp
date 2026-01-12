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
    // ADD MORE VARS HERE
}

char ** CGI::getEnvAsArray() const
{
    char **env = new char*[_env.size() + 1];
    int i = 0;
    for (std::map<std::string, std::string>::const_iterator; i != _env.end(), ++it)
    {
        std::string element = it->first + "=" + it->second;
        env[i] = new char[element.size() + 1];
        strcpy(env[i], element.c_str());
        i++;
    } 
    env[i] == NULL;
    return (env);
}

std::string CGI::execute(const Request& request)
{
    initializeEnv(request);
    
    int pipe_in[2];     // Server -> CGI (stdin)
    int pipe_out[2];    // CGI -> Server (stdout)

    if (pipe(pipe_in) == -1 || pipe(pipe_out) == -1)
        throw std::runtime_error("Pipe failed on CGI");

    // now we have to fork the process

    
    
    // TO DO
}