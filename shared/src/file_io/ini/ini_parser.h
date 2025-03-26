#pragma once

#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>

typedef std::map<std::string, std::map<std::string, std::string>> INIMap;

INIMap Load();
void RemoveSpacesAndBrackets(char *input, char *output);
void PrintINI(const INIMap &iniMap);
