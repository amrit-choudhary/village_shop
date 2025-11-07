#pragma once
#ifdef VG_WIN
/**
 * Input manager for Windows
 */

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <windows.h>

#include "input_manager.h"

namespace ME::Input {

class InputManagerWin : public ME::Input::PlatformInputManager {
   public:
    InputManagerWin();
    ~InputManagerWin();
    void Init() override;
    void Update(double deltaTime) override;
    void End() override;

    // Handle Windows-specific input messages.
    void HandleInput(UINT msg, WPARAM wParam, LPARAM lParam);

   private:
};
}  // namespace ME::Input

#endif  // VG_WIN
