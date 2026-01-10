/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rda-cunh <rda-cunh@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/12 19:11:39 by lmaes             #+#    #+#             */
/*   Updated: 2026/01/10 03:40:14 by rda-cunh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Utils.hpp"

void printMsg(std::string str)
{
	if (DBG_MSG == 1)
	{
		std::cout << DBG_COLOR << str << RESET << std::endl;
	}
	
}

// helper to extract value from the body of a request (form)
std::string getFormValue(const std::string &body, const std::string &key)
{
    std::string search = key + "=";
    size_t pos = body.find(search);
    if (pos == std::string::npos)
        return "";
    
    pos += search.length();
    size_t end = body.find("&", pos);
    if (end == std::string::npos)
        return body.substr(pos);
    
    return body.substr(pos, end - pos);
}

