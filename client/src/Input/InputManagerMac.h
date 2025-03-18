#ifdef VG_MAC
/**
 * Input manager or Mac
 */

#pragma once

#include <ApplicationServices/ApplicationServices.h>
#include <CoreFoundation/CoreFoundation.h>

#include <atomic>
#include <chrono>
#include <iostream>
#include <thread>

#include "InputManager.h"

class InputManagerMac : public PlatformInputManager {
 public:
  void Init() override;
  void Update(double deltaTime) override;
  void End() override;

 private:
  std::thread inputThread;
  static CGEventRef InputCallback(CGEventTapProxy proxy, CGEventType type,
                                  CGEventRef event, void *refcon);
  void StartEventLoop();
};

#endif  // VG_MAC
