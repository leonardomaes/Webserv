/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rda-cunh <rda-cunh@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/11 20:21:47 by rda-cunh          #+#    #+#             */
/*   Updated: 2026/01/14 00:59:58 by rda-cunh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/CGI.hpp"

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
    _env["QUERY_STRING"] = request.getQuery();                              // Does not exits in request class
    _env["CONTENT_LENGTH"] = request.getHeaderContent("Content-Length");    // Does not exits in request class
    _env["CONTENT_TYPE"] = request.getHeaderContent("Content-Type");        // Does not exits in request class
    _env["SCRIPT_NAME"] = _scriptPath;
    _env["SCRIPT_FILENAME"] = _scriptPath;
    _env["PATH_INFO"] = request.getPathTarget();
    _env["SERVER_PROTOCOL"] = "HTTP/1.1";
    _env["REDIRECT_STATUS"] = "200";
    // ADD MORE VARS HERE
}

char** CGI::getEnvAsArray() const
{
    char **env = new char*[_env.size() + 1];
    int i = 0;
    for (std::map<std::string, std::string>::const_iterator it = _env.begin(); it != _env.end(); ++it)
    {
        std::string element = it->first + "=" + it->second;
        env[i] = new char[element.size() + 1];
        strcpy(env[i], element.c_str());
        i++;
    } 
    env[i] = NULL;
    return (env);
}

std::string CGI::execute(const Request& request)
{
    initializeEnv(request);
    
    int pipe_in[2];     // Server -> CGI (stdin)
    int pipe_out[2];    // CGI -> Server (stdout)

    if (pipe(pipe_in) == -1 || pipe(pipe_out) == -1)
        throw std::runtime_error("Pipe failed on CGI");

    pid_t pid = fork();
    if (pid == -1)
        throw std::runtime_error("Fork failed on CGI");

    // child process
    if (pid == 0)
    {
        close(pipe_in[1]);  // close pipe in write end as child does not write to stdin
        close(pipe_out[0]); // close pipe out read end as child does not read from stdout

        dup2(pipe_in[0], STDIN_FILENO);     // redirect stdin
        dup2(pipe_out[1], STDOUT_FILENO);   // redirect stdout

        // preparing execve args and running it
        char *args[] = 
        {
            const_cast<char *>(_interepreterPath.c_str()),
            const_cast<char *>(_scriptPath.c_str()),
            NULL
        };
        char ** envp = getEnvAsArray();
        execve(args[0], args, envp);
        exit(1); // execve failed
    }
    else    // parent process
    {
        close(pipe_in[0]);  // parent does not need to read stdin
        close(pipe_out[1]); // parent does not need to write stdout

        // send POST body to CGI if exists
        if (request.getMethod() == "POST")
            write(pipe_in[1], request.getBody().c_str(), request.getBody().size());
        close(pipe_in[1]);

        // read output
        char buffer[4096];
        std::string result;
        ssize_t bytesRead;
        while ((bytesRead = read(pipe_out[0], buffer, sizeof(buffer))) > 0)
            result.append(buffer, bytesRead);
        close(pipe_out[0]);
        waitpid(pid, NULL, 0);  // wait for child
        return (result);
    }
}