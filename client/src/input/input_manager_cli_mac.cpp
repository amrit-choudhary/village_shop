#ifdef VG_MAC

#include "input_manager_cli_mac.h"

#include <iostream>

using namespace ME::Input;

void InputManagerCLIMac::StartInputLoop() {
    std::string tempInput;
    while (true) {
        std::getline(std::cin, tempInput);
        {
            std::lock_guard<std::mutex> lock(inputMutex);
            lastInput = tempInput;
        }
    }
}

void InputManagerCLIMac::Init() {
    std::cout << "CLI Init" << '\n';
    inputThread = std::thread(&InputManagerCLIMac::StartInputLoop, this);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

void InputManagerCLIMac::Update(double deltaTime) {
    return;
}

void InputManagerCLIMac::End() {
    std::cout << "CLI End" << '\n';
}

bool InputManagerCLIMac::GetCLIInputString(std::string& input) {
    {
        std::lock_guard<std::mutex> lock(inputMutex);
        if (lastInput.empty() == true) {
            return false;
        } else {
            input = lastInput;
            lastInput.clear();
            return true;
        }
    }
    return false;
}

#endif  // VG_MAC