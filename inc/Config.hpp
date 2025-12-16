/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rda-cunh <rda-cunh@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 18:55:08 by rda-cunh          #+#    #+#             */
/*   Updated: 2025/11/14 18:55:08 by rda-cunh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <fstream>

/*
This class will be responsible for:
- Open and read the .conf file
- lexing the file into tokens
- Parsing and storing the info in specific structs (ServerConfig + LocationConfig)
- Providing access to data (read-only)
*/

// List of token types
enum TokenType {
	TOK_KEYWORD,
	TOK_LCURLY,
	TOK_RCURLY,
	TOK_EOF,
};

// Struct for each token
struct Token {
	TokenType 	type;
	std::string value; // e.g. server, location, etc. 
};
 
// Struct to store each "server" block info
struct ServerConfig {
	std::string		listen;
	std::string		host;
	std::string		server_name;
	std::string		root;
	std::string		index;
	std::string		error_page;
	std::size_t		client_max_body_size;

	// All the locations from this server
	std::vector<LocationConfig> locations;

	ServerConfig() : client_max_body_size(0) {}
};

// Struct to store each "location" block info
struct LocationConfig {
	std::string					path;
	std::string					root;
	std::vector<std::string>	allow_methods;
	std::string					redirect;
	bool						auto_index;
	std::string 				try_file;
	bool 						has_cgi;
	std::string					cgi_path;
	std::string					cgi_ext;
	std::string					upload_to;

	LocationConfig() : auto_index(false), has_cgi(false) {}
};

class Config
{
	private: 
		std::string 				_path;
		std::vector<ServerConfig>	_servers;

		std::ifstream				_file;
		char						_current_char;
		std::size_t					_line_nr;
		int							_bracket_depth;
		bool 						_seen_server;

		// 1. Lexer helping functions
		void						initLexer(); 	// open file, set _current_char, reset counters
		Token						nextToken();
		void        				consumeWhiteSpace();
		void						consumeComment();
		void 						consumeKeywors
		

	public:
		Config();
		Config(const Config& obj);
		// Config& operator=(const Config& obj);
		~Config();
		
		Config(const std::string path);
		bool parseConfig();     //parse config file and return true if sucess
};