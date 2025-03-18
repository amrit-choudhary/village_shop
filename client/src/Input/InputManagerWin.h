#ifdef VG_WIN
/**
 * Input manager or Mac
 */

#pragma once

#include <windows.h>

#include <atomic>
#include <chrono>
#include <iostream>
#include <thread>

#include "InputManager.h"

class InputManagerWin : public PlatformInputManager {
 public:
  InputManagerWin();
  ~InputManagerWin();
  void Init() override;
  void Update(double deltaTime) override;
  void End() override;

 private:
  std::thread inputThread;
  void InputCallback(HANDLE hInput);
};

#endif  // VG_WIN
