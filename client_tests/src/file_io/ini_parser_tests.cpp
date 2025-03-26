#include "ini_parser_tests.h"

#include <iostream>

#include "src/file_io/ini/ini_parser.h"

bool TEST::Test_INIParse() {
    std::cout << "TEST: Starting: INI Parser" << '\n';
    INIMap iniMap = Load();
    if (iniMap["logging"]["level"] == "info") {
        std::cout << "TEST: Successful" << '\n';
        return true;
    } else {
        std::cout << "TEST: Failed" << '\n';
        return false;
    }
}