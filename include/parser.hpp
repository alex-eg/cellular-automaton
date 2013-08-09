#ifndef _L_PARSER
#define _L_PARSER

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <algorithm>

typedef std::map <std::string, std::string> KeyValueStorage;

class Settings {
private:
    std::map <std::string, KeyValueStorage> sections;
public:
    void AddToSection(std::string section_name, std::string key, std::string value);
    std::string Find(std::string item, std::string section = std::string(""));
    void Clear();
    KeyValueStorage& operator[](std::string section_name);
    Settings& operator=(const Settings &right);
    
    Settings() {}
    ~Settings() {}

};

class Parser {
private:
public:
    Settings Parse(std::string filename);
    Parser() {}
    ~Parser() {}
    
};

#endif
