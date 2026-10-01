#pragma once

#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>

#include "shared/src/file_io/vfs.h"

namespace ME {

typedef std::map<std::string, std::map<std::string, std::string>> INIMap;

class INIParser {
   public:
    /**
     * Reads relPath from the given root and parses it. Defaults to the shipped settings file in resources/.
     */
    static INIMap Load(const char *relPath = "config/settings.ini", FileRoot root = FileRoot::Resources);

    /**
     * Parses ini text already in memory.
     */
    static INIMap Parse(const std::string &text);

    static void Print(const INIMap &iniMap);

   private:
    static void RemoveSpacesAndBrackets(char *input, char *output);
};

}  // namespace ME
