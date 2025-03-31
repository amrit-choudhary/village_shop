#ifdef VG_MAC
/**
 * Input manager for Mac
 */

#pragma once

#include <ApplicationServices/ApplicationServices.h>
#include <CoreFoundation/CoreFoundation.h>

#include <atomic>
#include <chrono>
#include <iostream>
#include <thread>

#include "input_manager.h"

namespace ME::Input {

class InputManagerMac : public PlatformInputManager {
   public:
    void Init() override;
    void Update(double deltaTime) override;
    void End() override;

   private:
    std::thread inputThread;
    static CGEventRef InputCallback(CGEventTapProxy proxy, CGEventType type, CGEventRef event, void *refcon);
    void StartEventLoop();
};

}  // namespace ME::Input

#endif  // VG_MAC
