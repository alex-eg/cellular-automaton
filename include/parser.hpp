#ifndef _L_PARSER
#define _L_PARSER

#include <iostream>
#include <fstream>
#include <string>
#include <map>

typedef std::map <std::string, std::string> KeyValueStorage;

class Settings {
private:
    std::map <std::string, KeyValueStorage> Sections;
public:
    void AddToSection(std::string section_name, std::string key, std::string value);
    void SearchInSection(std::string item, std::string section = std::string(""));
    void Clear();
    
    Settings() {}
    ~Settings() {}

};

class Parser {
private:
public:
    void Parse(std::string filename);
    Parser() {}
    ~Parser() {}
    
};

#endif
