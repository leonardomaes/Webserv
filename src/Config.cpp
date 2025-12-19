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
    _servers.clear();    // clears any previous parsed data (good practive)

    initLexer();         // initialize lexer, opens file and initiate counters

    try                  // here we look for the server keyword and we parse the full block inside
    {
        while (1)
        {
            Token tok = nextToken();

            if (tok.type == TOK_EOF)
                break;

            if (tok.type == TOK_KEYWORD && tok.value == "server")
            {
                Token braceTok = nextToken();       //checks if token after server is a '{'
                if (braceTok.type != TOK_LCURLY)
                    throw ParseException("line" + ntos(_line_nr) + ": expected '{' after 'server'");            
                
                ServerConfig server = parseServerBlock();
            }    

            // to continue the logic here...
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
    if (dotPos == std::string::npos)
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
    _current_char = (c == EOF) ? '\0' : static_cast<char>(c);    
}

//This funtion returns the next token from the stream. Only returns a token if it is a valid keyword (TOK_KEYWORD)
Token Config::nextToken()
{
    std::string value;

    while (_file.good())
    {
        // consume and ignore new lines
        if (_current_char == '\n')  
        {
            ++_line_nr;
            int c = _file.get();
            _current_char = (c == EOF) ? '\0' : static_cast<char>(c);
            continue;
        }
        // skip whitespace
        if (std::isspace(static_cast<unsigned char>(_current_char)))
        {
            consumeWhiteSpace();
            continue;
        }
        // skip comments (start with #)
        if (_current_char == '#')
        {
            consumeComment();
            continue;
        }
        // left curly brace
        if (_current_char == '{')
        {
            ++_bracket_depth;
            int c = _file.get();
            _current_char = (c == EOF) ? '\0' : static_cast<char>(c);
            Token t;
            t.type = TOK_LCURLY;
            t.value = "{";
            return (t); 
        }
        // right curly brace
        if (_current_char == '}')
        {
            --_bracket_depth;
            int c = _file.get();
            _current_char = (c == EOF) ? '\0' : static_cast<char>(c);
            Token t;
            t.type = TOK_RCURLY;
            t.value = '}';
            return (t);
        }
        // keywords
        if (std::isalpha(static_cast<unsigned char>(_current_char)))
        {
            value.clear();
            value += _current_char;
            int c = _file.get();
            _current_char = (c == EOF) ? '\0' : static_cast<char>(c);
            
            // we are not calling consumeKeyword here (like above), we just complete the keyword
            while (_file.good() &&
                (std::isalnum(static_cast<unsigned char>(_current_char)) ||
                _current_char == '_'))
            {
                value += _current_char;
                c = _file.get();
                _current_char = (c == EOF) ? '\0' : static_cast<char>(c);
            }
            
            // decided to accept any word as a keyword and let the parser check if the keyword is valide later 
            // but we can change this behaviour later if needed
            if (value == "server")
                _seen_server = true;

            Token t;
            t.type = TOK_KEYWORD;
            t.value = value;
            return (t);
        }
        // for any other unusual character (not whitspace, comment, brace, etc.) we simply throw an exception
        std::string msg = "line " + ntos(_line_nr) + ": unexpected character '";
        msg += _current_char;
        msg += "'";
        throw ParseException(msg);
    }

    // some EOF final validations and EOF token
    if (_bracket_depth != 0)
        throw ParseException("config file: uneven curly brackets");
    if (!_seen_server)
        throw ParseException("config file: no server block was found");
    
    Token t;
    t.type = TOK_EOF;
    t.value = "";
    return (t);    
}

void Config::consumeWhiteSpace()
{
    while (_file.good() &&
        std::isspace(static_cast<unsigned char>(_current_char)))
    {
        if (_current_char == '\n')
            return; //allow nextToken to increment Line Numeber
        int c = _file.get();
        _current_char = (c == EOF) ? '\0' : static_cast<char>(c);
    }
}

void Config::consumeComment()
{
    while (_file.good() && _current_char != '\n')
    {
        int c = _file.get();
        _current_char = (c == EOF) ? '\0' : static_cast<char>(c);
    }
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////// PARSER HELPERS //////////////////////////////
////////////////////////////////////////////////////////////////////////////////

ServerConfig Config::parseServerBlock()
{
    // temp storage for the server parameters
    std::map<std::string, std::string> serverParams;

    // vector of all locations thar belong to this server
    std::vector<LocationConfig> locations;

    // parse the server body part
    parseServerBody(serverParams, locations);

    // converts maps of info into a structured ServerConfig (with validations)
    // returns that Serverconfig
    return (buildServerConfig(serverParams, locations));
}

void Config::parseServerBody(std::map<std::string, std::string> &serverParams, std::vector<LocationConfig> &locations)
{
    while (1)
    {
        Token tok = nextToken();

        if (tok.type == TOK_RCURLY)
            break;  //condition to end this server block

        if (tok.type != TOK_KEYWORD)
            throw ParseException("line " + ntos(_line_nr) + 
                                ": expected a keyword inside server block");
        
        // locations handling
        if (tok.value == "location")
        {
            // we need the location paht and its body
            // we use consumeKeyword to read "location <path> {" into a temp map
            std::map<std::string, std::string> locParams;
            std::string key = tok.value;
            consumeKeyword(key, locParams);
            // locParams["location"] now holds the path string
            std::string path = locParams["location"];

            LocationConfig loc = parseLocationBlock(path);
            locations.push_back(loc);
        }
        else
        {
            // reuse consumeKeyword to fill serverParams
            std::string key = tok.value;
            consumeKeyword(key, serverParams);
        }
    }
}