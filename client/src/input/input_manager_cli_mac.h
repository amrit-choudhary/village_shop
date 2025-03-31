#ifdef VG_MAC
/**
 * Input manager for CLI for Mac
 */

#pragma once

#include <mutex>
#include <string>
#include <thread>

#include "input_manager.h"

namespace ME::Input {

class InputManagerCLIMac : public PlatformInputManager {
   public:
    void Init() override;
    void Update(double deltaTime) override;
    void End() override;

    bool GetCLIInputString(std::string& input) override;

   private:
    void StartInputLoop();

    std::string lastInput;
    std::mutex inputMutex;
    std::thread inputThread;
};

}  // namespace ME::Input

#endif  // VG_MAC