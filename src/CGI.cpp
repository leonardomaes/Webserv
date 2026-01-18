/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rda-cunh <rda-cunh@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/11 20:21:47 by rda-cunh          #+#    #+#             */
/*   Updated: 2026/01/18 00:16:07 by rda-cunh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/CGI.hpp"

////////////////////////////////////////////////////////////////////////////////
///////////////////////////////// CONSTRUCTORS /////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

CGI::CGI(const std::string &scriptPath, const std::string &interpreterPath) :
    _scriptPath(scriptPath),
    _interpreterPath(interpreterPath)
{ }

CGI::~CGI() {}

////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////// FUNCTIONS //////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

void CGI::initializeEnv(const Request &request)
{
    _env["REQUEST_METHOD"] = request.getMethod();
    _env["QUERY_STRING"] = request.getQuery();                              
    _env["CONTENT_LENGTH"] = request.getHeaderContent("Content-Length");    
    _env["CONTENT_TYPE"] = request.getHeaderContent("Content-Type");        
    _env["SCRIPT_NAME"] = _scriptPath;
    _env["SCRIPT_FILENAME"] = _scriptPath;
    _env["PATH_INFO"] = request.getPathTarget();
    _env["SERVER_PROTOCOL"] = "HTTP/1.1";
    _env["REDIRECT_STATUS"] = "200";
    _env["HTTP_COOKIE"] = request.getHeaderContent("Cookie");
    _env["GATEWAY_INTERFACE"] = "CGI/1.1";
    _env["SERVER_SOFTWARE"] = "Webserv/1.0";
    _env["REMOTE_ADDR"] = "127.0.0.1"; 
    _env["SERVER_NAME"] = request.getHeaderContent("Host");
    _env["SERVER_PORT"] = request.getConfig()->listen;
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
        close(pipe_in[1]);                  // close pipe in write end as child does not write to stdin
        close(pipe_out[0]);                 // close pipe out read end as child does not read from stdout

        dup2(pipe_in[0], STDIN_FILENO);     // redirect stdin
        dup2(pipe_out[1], STDOUT_FILENO);   // redirect stdout

        close(pipe_in[0]);                  // safe practice
        close(pipe_out[1]);                 // safe practice

        // preparing execve args and running it
        char *args[] = 
        {
            const_cast<char *>(_interpreterPath.c_str()),
            const_cast<char *>(_scriptPath.c_str()),
            NULL
        };
        char ** envp = getEnvAsArray();
        execve(args[0], args, envp);
        exit(1);    // execve failed
    }
    else    // parent process
    {
        close(pipe_in[0]);  // parent does not need to read stdin
        close(pipe_out[1]); // parent does not need to write stdout

        // send POST body to CGI if exists
        if (request.getMethod() == "POST")
            write(pipe_in[1], request.getBody().c_str(), request.getBody().size());
        close(pipe_in[1]);

        // setup select for timeout
        fd_set read_fds;
        FD_ZERO(&read_fds);
        FD_SET(pipe_out[0], &read_fds);
        
        struct timeval timeout;
        timeout.tv_sec = 5;  // 5 seconds timeout
        timeout.tv_usec = 0;

        // wait until data available or timeout
        int ret = select(pipe_out[0] + 1, &read_fds, NULL, NULL, &timeout);

        if (ret == -1) 
        {
            kill(pid, SIGKILL);
            waitpid(pid, NULL, 0);
            close(pipe_out[0]);
            throw std::runtime_error("Select failed");
        }
        else if (ret == 0)           // TIMEOUT REACHED
        {
            kill(pid, SIGKILL);      // kill the script
            waitpid(pid, NULL, 0);   // cleanup zombie process
            close(pipe_out[0]);
            throw std::runtime_error("CGI Timeout");
        }

        // read output
        char buffer[4096];
        std::string result;
        ssize_t bytesRead;
        while ((bytesRead = read(pipe_out[0], buffer, sizeof(buffer))) > 0)
            result.append(buffer, bytesRead);
        close(pipe_out[0]);
        waitpid(pid, NULL, 0);      // wait for child
        return (result);
    }
}