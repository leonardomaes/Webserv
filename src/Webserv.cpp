/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rda-cunh <rda-cunh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/05 17:08:50 by lmaes             #+#    #+#             */
/*   Updated: 2026/01/19 16:25:15 by rda-cunh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Webserv.hpp"

Webserv::Webserv(Config* config)
{
    _epfd = epoll_create(1);
    if (_epfd < 0)
        throw std::runtime_error("epoll_create failed");

    const std::vector<ServerConfig>& servers = config->getServers();

    for (size_t i = 0; i < servers.size(); i++)
    {
        Server* srv = new Server(servers[i]);
        _servers.push_back(srv);

        struct epoll_event ev;
        memset(&ev, 0, sizeof(ev));
        ev.events = EPOLLIN;
        ev.data.fd = srv->getSocketFD();

        if (epoll_ctl(_epfd, EPOLL_CTL_ADD, srv->getSocketFD(), &ev) == -1)
            throw std::runtime_error("epoll_ctl ADD server socket failed");
    }
}

void Webserv::run()
{
	epoll_event events[MAX_EVENTS];
	while (g_running)
	{
		int eventsReady = epoll_wait(_epfd, events, MAX_EVENTS, -1);
		if (eventsReady < 0)
		{
			if (errno == EINTR) // if interrupted by a signal, ignore and go to the beggining of the loop (calling epoll_wait)
				continue;
			else
				throw std::runtime_error("epoll_wait failed");
		}
		for (int i = 0; i < eventsReady; i++)
		{
			int fd = events[i].data.fd;
			bool isServerSocket = false;
			Server* srv = NULL;
			for (size_t s = 0; s < _servers.size(); s++)
			{
				if (_servers[s]->getSocketFD() == fd)
				{
					isServerSocket = true;
					srv = _servers[s];
					break;
				}
			}
			if (isServerSocket)
			{
				int client_fd = srv->acceptClient();
				if (client_fd < 0)
					continue;
				// Epoll monitor client and stays till event
				// EPOLLIN
				// EPOLLET - Each trigger remove the actual event
				struct epoll_event ev;
				memset(&ev, 0, sizeof(ev));
				ev.events = EPOLLIN;
				ev.data.fd = client_fd;
				if (epoll_ctl(_epfd, EPOLL_CTL_ADD, client_fd, &ev) == -1)
				{ 
					close(client_fd);
					continue;
				}
				_clients[client_fd] = Client(client_fd, srv->getConfig());
				// _clients.insert(std::make_pair(client_fd, Client(client_fd, srv->getConfig())));
				continue;
			}
			if (events[i].events & EPOLLIN)
			{
				// std::map<int, Client>::iterator it = _clients.find(fd);
				// if (it == _clients.end())
				// 	continue;
				
				try
				{
					// it->second.readRequest(_epfd, fd, it->second.getConfig());
					// it->second.sendResponse(_epfd, fd);
					if (_clients[fd].readRequest(_epfd, fd, _clients[fd].getConfig()))
						_clients[fd].sendResponse(_epfd, fd);
				}
				catch(const std::exception& e)
				{
					_clients[fd].closeConnection(_epfd);
					_clients.erase(fd);
					std::cerr << "Client::" << e.what() << '\n';
				}
				
			}
		}
	}
	std::cout << "\nShutting down gracefully\n" << std::endl;
	// Close connection to clients
	for (int it = 0; it < MAX_CONNECTIONS; it++)
	{
		_clients[it].closeConnection(_epfd);
	}
	close(_epfd);
}

Webserv::~Webserv()
{
	for (size_t i = 0; i < _servers.size(); i++)
	{
		delete _servers[i];
	}
}

