/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmaes <lmaes@student.42porto.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 17:26:07 by lmaes             #+#    #+#             */
/*   Updated: 2025/10/27 17:26:07 by lmaes            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Request.hpp"

Request::Request()
{
}

Request::Request(std::string buffer)
{
	_firstLine = 1;
	std::string line;
	std::istringstream request(buffer);
	int code = 0;
	while (std::getline(request, line))
	{
		if (_firstLine)
			code = this->parseFirstLine(line);
		if (code != 0)
		{
			// Error case (TO ADD)
		}
	}
}

Request::~Request()
{
}


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////// FUNCTIONS ///////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

int Request::parseFirstLine(std::string line)
{
	for (size_t i = 0; line[i] != ' '; i++)
		this->_method.insert(_method.end(), line[i]);
	if (_method != "GET" && _method != "POST" && _method != "DELETE")
		return 400;
	return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////// GETTER /////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////



