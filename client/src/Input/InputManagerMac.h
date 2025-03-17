#ifdef VG_MAC
/**
 * Input manager or Mac
 */

#pragma once

#include <iostream>
#include <thread>
#include <atomic>
#include <chrono>
#include <CoreFoundation/CoreFoundation.h>
#include <ApplicationServices/ApplicationServices.h>

#include "InputManager.h"

class InputManagerMac : public PlatformInputManager
{
public:
    void Init() override;
    void Update(double deltaTime) override;
    void End() override;

private:
    std::thread inputThread;
    static CGEventRef InputCallback(CGEventTapProxy proxy, CGEventType type, CGEventRef event, void *refcon);
    void StartEventLoop();
};

#endif // VG_MAC
