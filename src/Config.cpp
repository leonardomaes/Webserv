/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rda-cunh <rda-cunh@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 18:55:22 by rda-cunh          #+#    #+#             */
/*   Updated: 2025/11/14 18:55:22 by rda-cunh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Config.hpp"
#include "../inc/Webserv.hpp"


////////////////////////////////////////////////////////////////////////////////
/////////////////////////////// HELPER FUNCTIONS ///////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// helper function to convert a number into a string using string stream (number to string)
// evaluate later to change it into an utilities cpp
static std::string ntos(std::size_t n)
{
    std::ostringstream oss;
    oss << n;
    return (oss.str());
}

////////////////////////////////////////////////////////////////////////////////
//////////////////////////// CANONICAL + CONSTRUCTOR ///////////////////////////
////////////////////////////////////////////////////////////////////////////////

Config::Config() :
    _path(""),
    _servers(),
    _file(),
    _current_char('\0'),
    _line_nr(0),
    _bracket_depth(0),
    _seen_server(false)
{ }

Config::Config(const std::string &path) :
    _path(path),
    _servers(),
    _file(),
    _current_char('\0'),
    _line_nr(0),
    _bracket_depth(0),
    _seen_server(false)
{ parseConfig(); }

Config::Config(const Config &other) :
    _path(other._path),
    _servers(other._servers),
    _file(),    // can't make a stream copy
    _current_char('\0'),
    _line_nr(0),
    _bracket_depth(0),
    _seen_server(false)
{ }

Config &Config::operator=(const Config &other)
{
 	if (this != &other)
	{
 		_path           = other._path;
        _servers        = other._servers;
        _current_char   = '\0';
        _line_nr        = 0;
        _bracket_depth  = 0;
        _seen_server    = false; 
 	}
 	return *this;
}

Config::~Config() 
{
    if (_file.is_open())
        _file.close();
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////// PARSER MAIN FUNCTION ////////////////////////////
////////////////////////////////////////////////////////////////////////////////

bool Config::parseConfig()
{
    _servers.clear()    // clears any previous parsed data (good practive)

    initLexer()         // initialize lexer, opens file and initiate counters

    try                 // here we look for the server keyword and we parse the full block inside
    {
        while (1)
        {
            Token tok = nextToken();

            if (toK.type == TOK_EOF)
                break;

            if (tok.type = TOK_KEYWORD && tok.value == "server")
            {
                Token braceTok = nextToken();       //checks if token after server is a '{'
                if (braceTok != TOK_LCURLY)
                    throw ParseException("line" + ntos(_line_nr) + ": expected '{' after 'server'");            
            }    
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Config parse error: " << e.what() << std::endl;     // check later with Leo if we somehow implement a more robust and integrated error module 
        return (false);
    }
}

////////////////////////////////////////////////////////////////////////////////
///////////////////////////////// LEXER - TOKENIZER ////////////////////////////
////////////////////////////////////////////////////////////////////////////////

void Config::initLexer()
{
    if (_file.is_open())        // guarantees file is close and can be opened later
        _file.close();


    // checking file extension 
    std::string::size_type dotPos = _path.find_last_of('.');
    if (dotPos == std::string:npos)
        throw ParseException("config file: no extension found");

    std::string extension = _path.substr(dotPos);
    if (extension != ".conf") 
        throw ParseException("config file: unknown extension");

    //open file and set counters and first char    
    _file.open((_path.c_str()));
    if (!_file.is_open())
        throw ParseException("config file: failed to open");

    _line_nr = 1;
    _bracket_depth = 0;
    _seen_server = false;

    int c = _file.get();
    _current_char = (c == EOF) ? '\0' : std::static_cast<char>(c);    
}




//getter for _servers
const std::vector<ServerConfig>& getServers() const { return _servers; }