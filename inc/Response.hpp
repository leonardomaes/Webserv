/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmaes <lmaes@student.42porto.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 17:24:09 by lmaes             #+#    #+#             */
/*   Updated: 2025/10/27 17:24:10 by lmaes            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Webserv.hpp"
#include "Request.hpp"

class Response
{
private:
	std::map<int, std::string> _status;
	std::string _root;
	void FillStatus();
	std::string getContent(Request obj);
	std::string getStatus(Request obj);
public:
	Response();
	Response(const Response& obj);
	// Response& operator=(const Response& obj);
	~Response();

// Functions
	void generateResponse(Request obj, int epfd, int eventFD);
// Getters
	std::string getRoot();
};