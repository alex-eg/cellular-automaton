#include <algorithm>
#include <iostream>
#include <fstream>

#include "parser.hpp"

void Settings::AddToSection(std::string sec_name, std::string key, std::string value)
{
    sections[sec_name][key] = value;
}

std::string Settings::Find(std::string item, std::string section)
{
    for (auto& kv_section : sections)
        for (auto& kv_parameter : kv_section.second)
            if (kv_parameter.first == item) return kv_parameter.second;
    return std::string("");
}

void Settings::Clear()
{
    for (auto& kv : sections)
        kv.second.clear();
    sections.clear();
}

KeyValueStorage& Settings::operator[](std::string section_name)
{
    return sections[section_name];
}

Settings& Settings::operator=(const Settings &right)
{
    Clear();
    for (auto& kv_section : right.sections)
        for (auto& kv_parameter : kv_section.second)
            sections[kv_section.first][kv_parameter.first] = kv_parameter.second;
    return *this;
}

Settings Parser::Parse(std::string filename)
{
    std::string line, current_section = std::string("");
    std::fstream inifile(filename.c_str());
    Settings settings;
    if (!inifile) std::cerr << "Cannot open file " << filename << std::endl;
    while (!inifile.eof()) {
        std::getline(inifile, line);
        if (!line.length()) continue;
        if (line[0] == '#') continue;
        if (line[0] == ';') continue;
        if (line[0] == '[') {
            if (line[line.length() - 1] != ']') throw("Malformed section name");
            current_section = line.substr(1, line.length() - 2);
            std::cout << "<<section changed: >>" << current_section << std::endl;
            continue;
        }
        std::string key = line.substr(0, line.find_first_of('=') - 1);
        std::string value = line.substr(line.find_first_of('=') + 1, line.length() - 1);
        settings[current_section][key] = value;
        std::cout << key << " ---- " << value << std::endl;
    }
    inifile.close();
    return settings;
}
