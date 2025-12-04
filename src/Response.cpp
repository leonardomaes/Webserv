/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmaes <lmaes@student.42porto.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 17:24:27 by lmaes             #+#    #+#             */
/*   Updated: 2025/10/27 17:24:28 by lmaes            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Response.hpp"

Response::Response()
{
	this->FillStatus();
}

Response::~Response()
{
}

void Response::FillStatus()
{
	// this->_status = {{100, "Continue"}, {101, "Switching Protocols"},
	// 		{102, "Processing"}, {103, "Early Hints"}, {200, "OK"}, {201, "Created"},
	// 		{202, "Accepted"}, {203, "Non-Authoritative Information"}, {204, "No Content"},
	// 		{205, "Reset Content"}, {206, "Partial Content"}, {207, "Multi-Status"},
	// 		{208, "Already Reported"}, {226, "IM Used"}, {300, "Multiple Choices"}, {301, "Moved Permanently"},
	// 		{302, "Found"}, {303, "See Other"}, {304, "Not Modified"}, {305, "Use Proxy"}, {306, "Unused"},
	// 		{307, "Temporary Redirect"}, {308, "Permanent Redirect"}, {400, "Bad Request"}, {401, "Unauthorized"},
	// 		{402, "Payment Required"}, {403, "Forbidden"}, {404, "Not Found"}, {405, "Method Not Allowed"},
	// 		{406, "Not Acceptable"}, {407, "Proxy Authentication Required"}, {408, "Request Timeout"}, {409, "Conflict"},
	// 		{410, "Gone"}, {411, "Length Required"}, {412, "Precondition Failed"}, {413, "Content Too Large"},
	// 		{414, "URI Too Long"}, {415, "Unsupported Media Type"}, {416, "Range Not Satisfiable"}, {417, "Expectation Failed"},
	// 		{418, "I'm a teapot"}, {421, "Misdirected Request"}, {422, "Unprocessable Content"}, {423, "Locked"},
	// 		{424, "Failed Dependency"}, {425, "Too Early"}, {426, "Upgrade Required"}, {428, "Precondition Required"},
	// 		{429, "Too Many Requests"}, {431, "Request Header Fields Too Large"}, {451, "Unavailable For Legal Reasons"},
	// 		{500, "Internal Server Error"}, {501, "Not Implemented"}, {502, "Bad Gateway"}, {503, "Service Unavailable"},
	// 		{504, "Gateway Timeout"}, {505, "HTTP Version Not Supported"}, {506, "Variant Also Negotiates"},
	// 		{507, "Insufficient Storage"}, {508, "Loop Detected"}, {510, "Not Extended"}, {511, "Network Authentication Required"}};

	_status[100] = "Continue";
	_status[101] = "Switching Protocols";
	_status[102] = "Processing";
	_status[103] = "Early Hints";

	_status[200] = "OK";
	_status[201] = "Created";
	_status[202] = "Accepted";
	_status[203] = "Non-Authoritative Information";
	_status[204] = "No Content";
	_status[205] = "Reset Content";
	_status[206] = "Partial Content";
	_status[207] = "Multi-Status";
	_status[208] = "Already Reported";
	_status[226] = "IM Used";

	_status[300] = "Multiple Choices";
	_status[301] = "Moved Permanently";
	_status[302] = "Found";
	_status[303] = "See Other";
	_status[304] = "Not Modified";
	_status[305] = "Use Proxy";
	_status[306] = "Unused";
	_status[307] = "Temporary Redirect";
	_status[308] = "Permanent Redirect";

	_status[400] = "Bad Request";
	_status[401] = "Unauthorized";
	_status[402] = "Payment Required";
	_status[403] = "Forbidden";
	_status[404] = "Not Found";
	_status[405] = "Method Not Allowed";
	_status[406] = "Not Acceptable";
	_status[407] = "Proxy Authentication Required";
	_status[408] = "Request Timeout";
	_status[409] = "Conflict";
	_status[410] = "Gone";
	_status[411] = "Length Required";
	_status[412] = "Precondition Failed";
	_status[413] = "Content Too Large";
	_status[414] = "URI Too Long";
	_status[415] = "Unsupported Media Type";
	_status[416] = "Range Not Satisfiable";
	_status[417] = "Expectation Failed";
	_status[418] = "I'm a teapot";
	_status[421] = "Misdirected Request";
	_status[422] = "Unprocessable Content";
	_status[423] = "Locked";
	_status[424] = "Failed Dependency";
	_status[425] = "Too Early";
	_status[426] = "Upgrade Required";
	_status[428] = "Precondition Required";
	_status[429] = "Too Many Requests";
	_status[431] = "Request Header Fields Too Large";
	_status[451] = "Unavailable For Legal Reasons";

	_status[500] = "Internal Server Error";
	_status[501] = "Not Implemented";
	_status[502] = "Bad Gateway";
	_status[503] = "Service Unavailable";
	_status[504] = "Gateway Timeout";
	_status[505] = "HTTP Version Not Supported";
	_status[506] = "Variant Also Negotiates";
	_status[507] = "Insufficient Storage";
	_status[508] = "Loop Detected";
	_status[510] = "Not Extended";
	_status[511] = "Network Authentication Required";
}

std::string Response::getContent(Request obj)
{
	std::ifstream file(obj.getPathTarget().c_str(), std::ios::in | std::ios::binary);
	if (!file)
		return "";

	std::string result;
	char buffer[4096];

	while (file.read(buffer, sizeof(buffer)) || file.gcount() > 0) {
		result.append(buffer, file.gcount());
	}

	return result;
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////// FUNCTIONS ///////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

void Response::generateResponse(Request obj, int epfd, int eventFD)
{
	std::cout << "DBG::" << obj.getPathTarget() << std::endl;
	std::cout << "DBG::" << obj.getCode() << std::endl;
	
	// *******************************************************************
	std::string body = this->getContent(obj);
	std::cout << "DBG::" << body << std::endl << std::endl;
	//std::string body = "<!DOCTYPE html>\r\nHello World!!!\r\n";
	std::stringstream ss;
	ss << body.size();
	std::string response =	"HTTP/1.1 200 OK\r\n"
							"Content-Type: text/html; charset=UTF-8\r\n"
							"Content-Length: " + ss.str() + "\r\n\r\n" +
							body;
	// *******************************************************************
	// TO DO
	// Send HTTP response	(RESPONSE)
	send(eventFD, response.c_str(), response.size(), 0);
	if (obj.getConnection() == "close")
	{
		// Delete event from epoll
		epoll_ctl(epfd, EPOLL_CTL_DEL, eventFD, NULL);
		// TO DO
		// Check line, if "Connection: keep-alive", we must not close it
		close(eventFD);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////// GETTER /////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

