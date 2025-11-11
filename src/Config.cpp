//missing header

#include "config.hpp"
#include "server.hpp"

Config::Config(const std::string path) : _path(path) {}

bool Config::parseConfig()
{
    std::ifstream file(_path.c_str()):
    if (!file.is_open())
    {
        std::cerr << "Could not open config file: " << _path << std::endl;
        return (false);
    }

    std::string line;
    int lineNum = 1;
    while (getline(file, line))     //simple version to print lines of the config file. This will evolve later into something bigger
    {
        std::out << "Line " << lineNum << ":" << line << std::endl;
        lineNum++;
    }
    return (true);

}