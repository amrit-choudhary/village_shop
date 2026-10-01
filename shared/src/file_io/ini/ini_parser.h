#pragma once

#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>

#include "shared/src/misc/utils.h"

typedef std::map<std::string, std::map<std::string, std::string>> INIMap;

/**
 * Loads baseDir + relPath. Defaults to the shipped settings file in resources/.
 */
INIMap Load(const std::string &relPath = "config/settings.ini",
            const std::string &baseDir = ME::Utils::GetResourcesPath());
void RemoveSpacesAndBrackets(char *input, char *output);
void PrintINI(const INIMap &iniMap);
