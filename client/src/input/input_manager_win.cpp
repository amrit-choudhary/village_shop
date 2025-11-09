#ifdef VG_WIN

#include "input_manager_win.h"

#include <src/logging.h>

#include "../misc/global_vars.h"

using namespace ME::Input;

// Native keycode (Virtual Key) to game KeyCode conversion map.
static std::unordered_map<uint16_t, KeyCode> KeyCodeMap = {

    {0x57, KeyCode::W},      {0x53, KeyCode::S},      {0x41, KeyCode::A},
    {0x44, KeyCode::D},      {0x25, KeyCode::LArrow}, {0x27, KeyCode::RArrow},
    {0x26, KeyCode::UArrow}, {0x28, KeyCode::DArrow}, {0x20, KeyCode::Space}};

InputManagerWin::InputManagerWin() {}

InputManagerWin::~InputManagerWin() {}

void InputManagerWin::Init() {}

void InputManagerWin::Update(double deltaTime) {}

void InputManagerWin::End() {}

void InputManagerWin::HandleInput(UINT msg, WPARAM wParam, LPARAM lParam) {
    // use pointer-sized type to avoid truncation on x64
    uintptr_t lp = static_cast<uintptr_t>(lParam);
    constexpr uintptr_t TRANSITION_MASK = (uintptr_t{1} << 31);  // 1 => transition = key released
    constexpr uintptr_t PREV_STATE_MASK = (uintptr_t{1} << 30);  // 1 => key was previously down

    bool isKeyUp = (lp & TRANSITION_MASK) != 0;
    bool wasPreviouslyDown = (lp & PREV_STATE_MASK) != 0;

    // On Key Press Down
    if (!isKeyUp && !wasPreviouslyDown) {
        KeyCode keyCode = KeyCodeMap[static_cast<uint16_t>(wParam)];
        if (keyCode != KeyCode::None) {
            InputManager::GlobalKeyState[keyCode] = KeyState::Down;
        }
    }

    // On Key Released
    if (isKeyUp && wasPreviouslyDown) {
        KeyCode keyCode = KeyCodeMap[static_cast<uint16_t>(wParam)];
        if (keyCode != KeyCode::None) {
            InputManager::GlobalKeyState[keyCode] = KeyState::Up;
        }
    }
}

#endif  // VG_WIN
