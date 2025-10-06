/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmaes <lmaes@student.42porto.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 19:36:46 by lmaes             #+#    #+#             */
/*   Updated: 2025/10/06 19:36:47 by lmaes            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Server.hpp"

Server::Server()
{
}

Server::Server(const Server& obj)
{
    (void)obj;
}

Server Server::operator=(const Server& obj)
{
	if (this != &obj)
	{
		*this = obj;
	}
	return *this;
}

Server::~Server()
{
}

//////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////// EXCEPTIONS ///////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////


Server::ServerException::ServerException(const std::string& error)
{
	_errorMsg = "Error: " + error;
}

Server::ServerException::~ServerException() throw() {}


const char *Server::ServerException::what() const throw() {
	return _errorMsg.c_str();
}

