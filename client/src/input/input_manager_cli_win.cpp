#ifdef VG_WIN

#include "input_manager_cli_win.h"

#include <iostream>

using namespace ME::Input;

void InputManagerCLIWin::StartInputLoop() {
    std::string tempInput;
    while (true) {
        std::getline(std::cin, tempInput);
        {
            std::lock_guard<std::mutex> lock(inputMutex);
            lastInput = tempInput;
        }
    }
}

void InputManagerCLIWin::Init() {
    std::cout << "CLI Init" << '\n';
    inputThread = std::thread(&InputManagerCLIWin::StartInputLoop, this);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

void InputManagerCLIWin::Update(double deltaTime) {
    return;
}

void InputManagerCLIWin::End() {
    std::cout << "CLI End" << '\n';
}

bool InputManagerCLIWin::GetCLIInputString(std::string& input) {
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

#endif  // VG_WIN