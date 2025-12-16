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
		Token						nextToken();	// goes over tokens and split them
		void        				consumeWhiteSpace();
		void						consumeComment();
		bool 						consumeKeywords(std::string &token_value, 
										std::map<std::string, std::string> &parameters); 


		// 2. Parser helping functions (make sense of the tokes and store it in the structs)
		
		/*
		parseConfig() return a ServerConfig structure filled with the server info:
		 - Use a temporary std::map<std::string, std::string> parameters and collects all data 
		   like "listen" "host" "root";
		 - On a separate struct separate all the locations the are found inside the server block;
		 - Validate data abd build a ServerConfig from parameters + locations. 		
		*/
	    ServerConfig parseServerBlock();

		/*
		parseServerBody() idea is to parse all directives that are inside a sever block:
		 - follows kuninoto Lexer::consumeKeyword logic (and mix with Server::Server logic);
		 - when it findes a location, it delegates the parsing into parseLocationBlock();
		*/
		void parseServerBody(std::map<std::string, std::string> &serverParams, 
			stinitLexerd::vector<LocationConfig> &locations);

		/*
		parseLocationBlock() parses "location <path> { ... }" block pattern and returns a LocationConfig struct. It must:
		 - allow only server forbiden keywords (allow_methods, cgi_path, ...)
		 - allow only one falue for each parameter (except for allow_methods) 
		*/
		void parseLocationBlock(const std::string &firstLocationPath):

		/*
		buildServerConfig() grabs all the info (parameters and locations), validates them and delivers the ServerConfig data;
		Validations made:
		 - All needed info is present;
		 - Forbidden keywords at the Server level;
		 - Normalize info (e.g. normalize root, check error_page path, parse numbers, etc.)
		*/
		ServerConfig buildServerConfig(const std:map<std::sting, std::string> &serverParams,
			std::vector<LocationConfig> &locations) const;

	public:

		/*
		Each Config object created will be connect to a specific config file;
		TO DECIDE: if the parser is iniciated in the constructor (automatic when object is created) or independently (need to be called)
		*/
		Config(const std::string path);

		Config();
		Config(const Config &other);
		Config& operator=(const Config& other);
		~Config();

		/*
		parseConfig() simply parses the config file. Steps:
		 - Initialize the initLexer() function on _path;
		 - Parse repeatedly the server {...} blocks int ServerConfig objects;
		 - Store them in the _servers vector.
		Returns 1 on sucess or thows an exception if the parsing fails.
		*/
		bool parseConfig();

		/*
		Getter for access the server configurations. Each entry Server confog corresponds to a "server { ... }" block from the file. 
		NOTE FOR LATER: Check later if needed. 
		*/
		const std::vector<ServerConfig> &getServers() const;



		Class ParseException : public std::exception
		{
			public:
		}



    class ParseException : public std::exception {
    public:
        explicit ParseException(const std::string& msg) : _msg(msg) {}
        virtual const char* what() const throw() { return _msg.c_str(); }
        ~ParseException() throw() {}
    private:
        std::string _msg;
    };






		


};