#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <map>

typedef std::map<std::string, std::map<std::string, std::string>> INIMap;

INIMap Load();
void RemoveSpacesAndBrackets(char *input, char *output);
void PrintINI(const INIMap& iniMap);
