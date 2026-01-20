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
#include "../inc/Header.hpp"

////////////////////////////////////////////////////////////////////////////////
/////////////////////////////// HELPER FUNCTIONS ///////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// helper function to convert a number into a string with string stream
static std::string ntos(std::size_t n)
{
    std::ostringstream oss;
    oss << n;
    return (oss.str());
}

static bool isOnlyWhiteSpaces(const std::string &s)
{
    for (std::string::const_iterator it = s.begin(); it != s.end(); ++it)
    {
        if (!std::isspace(static_cast<unsigned char>(*it)))
            return (false);
    }
    return (true);
}

// trim whitespaces on the beggining and end of a string 
static std::string trim(const std::string &s)
{
    size_t start = 0;
    size_t end = s.length();
    
    // first non-whitespace
    while (start < end && std::isspace(static_cast<unsigned char>(s[start])))
        ++start;
    
    // last non-whitespace
    while (end > start && std::isspace(static_cast<unsigned char>(s[end - 1])))
        --end;
    
    return s.substr(start, end - start);
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
{ 
    parseConfig();
    #ifdef DEBUG_CONFIG
    debugPrintAllServers();
    #endif
}

Config::Config(const Config &other) :
    _path(other._path),
    _servers(other._servers),
    _file(),                    // can't make a stream copy
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
                Token braceTok = nextToken();       // checks if token after server is a '{'
                if (braceTok.type != TOK_LCURLY)
                    throw ParseException("line" + ntos(_line_nr) + ": expected '{' after 'server'");            
                
                ServerConfig server = parseServerBlock();
                _servers.push_back(server);         //store info in _servers
            }
            else
            {
                // Minimal grammar rule: anything else at top level is invalid
                throw ParseException("line " + ntos(_line_nr) + ": unexpected token \"" + tok.value + "\"");
            }
        }
        // if we reach here parsing is succeded (more checks can be added here (e.g. duplicated listen))
        return (true);
    }
    catch (const std::exception &e)
    {
		std::stringstream ss;
		ss << e.what();
		throw std::runtime_error("Config parse error: " + ss.str());
    }
    return (true);
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
            return;         //allow nextToken to increment Line Numeber
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

/* this function fills parameters at a server or at a location level:
    - assumes a token_value that contains the keywod name
    - reads if the value until ';' or '}'
    - populates a map parameters[token_value] = parameter_value */
void Config::consumeKeyword(std::string &token_value, std::map<std::string, std::string> &parameters)
{
    std::string parameter_value;

    // after reading the keyword, _current_char is the next char from file, so we skip the space after keyword
    if (_current_char == ' ')
        consumeWhiteSpace();

    if (_current_char == ';')
        throw ParseException("line " + ntos(_line_nr) + ": no value found for keyword \"" + token_value + "\"");
    
    // special case for handling "location" <path> "{"
    if (token_value == "location")
    {
        while (_file.good() && _current_char != '{')
        {
            if (_current_char == '\n')
                throw ParseException("line " + ntos(_line_nr) + ": missing '{' after location");
            parameter_value += _current_char;
            int c = _file.get();
            _current_char = (c == EOF) ? '\0' : static_cast<char>(c);
        }
        
        if (_current_char == '{')
        {
            ++_bracket_depth;
            int c = _file.get();
            _current_char = (c == EOF) ? '\0' : static_cast<char>(c);
        }

        if (parameter_value.empty() || isOnlyWhiteSpaces(parameter_value))
            throw ParseException("line: " + ntos(_line_nr) + ": no value found for keyword \"" + token_value + "\"");
        parameters[token_value] = parameter_value;
        return;      
    }

    // for the generic case keyword <value> [this handles the case for one value only]
    if (_current_char == ' ')
        consumeWhiteSpace();
    
    // continue adding the value from the file into parameter_value
    while(_file.good() && _current_char != ';')
    {
        if (_current_char == '\n')
            throw ParseException("line " + ntos(_line_nr) + ": Missing ';' after \"" + token_value + "\"");
        parameter_value += _current_char;
        int c = _file.get();
        _current_char = (c == EOF) ? '\0' : static_cast<char>(c);
    }
    
    //parameters map entry
    parameters[token_value] = parameter_value;
    
    // consume ';'
    int c = _file.get();
    _current_char = (c == EOF) ? '\0' : static_cast<char>(c);
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
            // we need the location path and its body
            // we use consumeKeyword to read "location <path> {" into a temp map
            std::map<std::string, std::string> locParams;
            std::string key = tok.value;
            consumeKeyword(key, locParams);
            // locParams["location"] now holds the path string
            std::string path = locParams["location"];

            LocationConfig loc = parseLocationBlock(path);
            locations.push_back(loc);
        }
        else if (tok.value == "error_page")
        {
            // collects all error pages entries lines
            std::map<std::string, std::string>::iterator it = serverParams.find("error_page");
            std::map<std::string, std::string> tmp;
            std::string key = tok.value;
            consumeKeyword(key, tmp);
            if (it == serverParams.end())
                serverParams["error_page"] = tmp["error_page"];
            else
                it->second += "\n" + tmp["error_page"];
        }
        else
        {
            // reusing consumeKeyword to fill serverParams
            std::string key = tok.value;
            consumeKeyword(key, serverParams);
        }
    }
}

LocationConfig Config::parseLocationBlock(const std::string &firstLocationPath)
{
    LocationConfig loc;
    loc.path = trim(firstLocationPath);

    // creating a temporary map for this location raw info (root, allow_methods, etc...)
    std::map<std::string, std::string> locParams;

    while (1)
    {
        Token tok = nextToken();

        if (tok.type == TOK_RCURLY)
            break; // break the loop when the location block ends
        
        if (tok.type != TOK_KEYWORD)
            throw ParseException("line " + ntos(_line_nr) + ": expected keyword not found inside location block");
        
        std::string key = tok.value;
        consumeKeyword(key, locParams);
    }

    // converts locParams temp map into the LocationConfig fields
    std::map<std::string, std::string>::const_iterator it;

    it = locParams.find("root");
    if (it != locParams.end())  //if key is found place it in the LocationConfig struct
        loc.root = it ->second;
    
    it = locParams.find("redirect");
    if (it != locParams.end())
        loc.redirect = it->second;
    
    it = locParams.find("try_file");
    if (it != locParams.end())
        loc.try_file = it->second;
    
    it = locParams.find("upload_to");
    if (it != locParams.end())
        loc.upload_to = it->second;

    it = locParams.find("auto_index");
    if (it != locParams.end())
        loc.auto_index = (it->second == "on" || it->second == "true"); // check later if this use is enough (mimmics ngix autoindex)
    
    it = locParams.find("cgi_path");
    if (it != locParams.end())
    {
        loc.has_cgi = true;
        loc.cgi_path = it->second;
    }

    it = locParams.find("cgi_ext");
    if (it != locParams.end())
        loc.cgi_ext = it->second;

    it = locParams.find("index");
    if (it != locParams.end())
        loc.index = it->second;
    
    it = locParams.find("allow_methods");
    if (it != locParams.end())
    {
        // simple parser that parses methods by spaces
        std::string value = it->second;
        std::string current;
        for (size_t i = 0; i < value.size(); ++i) 
        {
            char ch = value[i];
            if (ch == ' ') 
            {
                if (!current.empty()) 
                {
                    loc.allow_methods.push_back(current);
                    current.clear();
                }
            } 
            else 
            {
                current += ch;
            }
        }
        if (!current.empty())
            loc.allow_methods.push_back(current);
    }
    return (loc);
}

/* Build a ServerConfig from serverParams (temp map) and locations
Here we also implement additions validations needed
	- mandatory directives needed in the config file
	- forbiden directives in server
	- normalization, number validation, etc. */
ServerConfig Config::buildServerConfig(const std::map<std::string, std::string> &serverParams, const std::vector<LocationConfig> &locations) const 
{
	ServerConfig conf;
    std::map<std::string, std::string>::const_iterator it;

	// only mandatory keys (we can adapt and add more later)
	static const char* mandatoryKeys[] = {"listen", "root"};
	const size_t mandatoryCount = sizeof(mandatoryKeys) / sizeof(mandatoryKeys[0]);

    for(size_t i = 0; i < mandatoryCount; ++i)
    {
        if (serverParams.find(mandatoryKeys[i]) == serverParams.end())
            throw ParseException("no provided value for " + std::string(mandatoryKeys[i]));
    }

    // mandatory fields
    conf.listen = trim(serverParams.find("listen")->second);
    conf.root   = trim(serverParams.find("root")->second);

    it = serverParams.find("host");
    conf.host = (it != serverParams.end()) ? trim(it->second) : "0.0.0.0"; // key value or default

    it = serverParams.find("index");
    conf.index = (it != serverParams.end()) ? trim(it->second) : "index.html"; // key value or default

    it = serverParams.find("server_name");
    if (it != serverParams.end())
        conf.server_name = trim(it->second);
      
    // error_page special parsing to store the info in a map    
    it = serverParams.find("error_page");
    if (it != serverParams.end())
    {
        // splitting different lines as we store them all together in serverParams separated by '\n'
        std::string all = it->second;
        std::string line;
        std::string::size_type start = 0;

        while (start <= all.size())
        {
            std::string::size_type pos = all.find('\n', start);
            if (pos == std::string::npos)
                line = all.substr(start);
            else
                line = all.substr(start, pos - start);
        
            line = trim(line);
            if (!line.empty())
            {
                // parsing line into tokens sepaarated by spaces (line is like "413 400 /40x.html")
                std::vector<std::string> parts;
                std::string current;
                for (size_t i = 0; i < line.size(); ++i)
                {
                    char ch = line[i];
                    if (ch == ' ')
                    {
                        if (!current.empty())
                        {
                            parts.push_back(current);
                            current.clear();
                        }
                    }
                    else
                        current += ch;
                }
                if (!current.empty())
                    parts.push_back(current);

                if (parts.size() < 2)
                    throw ParseException("invalid error_page content \"" + line + "\"");
                
                // last token is always the path and previous tokens are the codes
                const std::string path = parts[parts.size() - 1];

                for (size_t i = 0; i + 1 < parts.size(); ++i) // convert all codes int intigers
                {
                    std::istringstream iss(parts[i]);
                    int code = 0;
                    if (!(iss >> code) || !iss.eof())
                        throw ParseException ("invalid error code \"" + parts[i] + "\" in error_pages");
                    conf.error_pages[code] = path;  // it inserts or overrides if already exists
                }           
            }
    
        if (pos == std::string::npos)
            break;
        start = pos + 1; 
        }
    }

    it = serverParams.find("client_max_body_size");
    if (it != serverParams.end())
    {
        // very simple parsing that can be updated later (if needed). Now it accepts only numbers in bytes. 
        std::istringstream iss(trim(it->second));
        std::size_t sz = 0;
        if (!(iss >> sz) || !iss.eof())
            throw ParseException("invalid client_max_body_size value \"" + it->second + "\"");
        conf.client_max_body_size = sz;
    }

    // atach locations
    conf.locations = locations;

    // We can add further checks (e.g. ports range) if needed
    return conf;

}

////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////// GETTERS ////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

const std::vector<ServerConfig> &Config::getServers() const
{
    return (_servers);
}

ServerConfig Config::getServerConfig(int index)
{
	return _servers[index];
}

////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////// DEBUG /////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

void LocationConfig::debugPrint() const
{
    #if DBG_MSG
    std::cout << "path: " << path << "\n";
    std::cout << "root: " << root << "\n";
    std::cout << "redirect: " << redirect << "\n";
    std::cout << "auto_index: " << auto_index << "\n";
    std::cout << "try_file: " << try_file << "\n";
    std::cout << "upload_to: " << upload_to << "\n";
    std::cout << "cgi_path: " << cgi_path << "\n";
    std::cout << "cgi_ext: " << cgi_ext << "\n";
    std::cout << "index: " << index << "\n";
    std::cout << "allow_methods: ";
    for (size_t i = 0; i < allow_methods.size(); ++i)
        std::cout << allow_methods[i] << " ";
    std::cout << "\n";
    #endif
}

void ServerConfig::debugPrint() const
{
    #if DBG_MSG
    std::cout << "=== SERVER CONFIG ===\n";
    std::cout << "listen: " << listen << "\n";
    std::cout << "host: " << host << "\n";
    std::cout << "server_name: " << server_name << "\n";
    std::cout << "root: " << root << "\n";
    std::cout << "index: " << index << "\n";
    std::cout << "client_max_body_size: " << client_max_body_size << "\n";
    std::cout << "locations count: " << locations.size() << "\n";
    
    std::cout << "error_pages:\n";
    for (std::map<int, std::string>::const_iterator it = error_pages.begin();
            it != error_pages.end(); ++it)
            std::cout << " " << it->first << " -> " << it->second << "\n";

    for (size_t i = 0; i < locations.size(); ++i)
    {
        std::cout << "\n--- Location " << i << " ---\n";
        locations[i].debugPrint();
    }
    std::cout << "=====================\n";
    #endif
}

void Config::debugPrintAllServers() const
{
    #if DBG_MSG
    for (size_t i = 0; i < _servers.size(); ++i)
    {
        std::cout << "\n### SERVER " << i << " ###\n";
        _servers[i].debugPrint();
    }
    #endif
}