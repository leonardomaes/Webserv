//missing header

#pragma once

#include <string>
#include <fstream>

class Config
{
    private: 
        std::string _path;

    public:
        Config(const std::string path);
        bool parseConfig();     //parse config file and return true if sucess
};