/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmaes <lmaes@student.42porto.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/05 17:08:50 by lmaes             #+#    #+#             */
/*   Updated: 2026/01/05 17:08:51 by lmaes            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Server.hpp"

class Server;
class Client;

class Webserv
{
private:
	int							_epfd;
	std::vector<Server*>		_servers;
	std::map<int, Client>		_clients;
	Config*						_config;
public:
	Webserv(Config* config);
	~Webserv();

	void run();
};
