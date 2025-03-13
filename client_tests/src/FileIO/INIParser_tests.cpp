#include <iostream>

#include "INIParser_tests.h"

bool TEST::Test_INIParse()
{
    std::cout << "TEST: Starting: INI Parser" << '\n';
    INIMap iniMap = Load();
    if(iniMap["logging"]["level"] == "info"){
        std::cout << "TEST: Successful" << '\n';
        return true;
    }
    else{
        std::cout << "TEST: Failed" << '\n';
        return false;
    }
}