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

InputManagerWin::InputManagerWin() {
    bufferFirst = new WinKeyEventBuffer();
    bufferSecond = new WinKeyEventBuffer();
    readBuffer = bufferFirst;
    writeBuffer = bufferSecond;
    bReadingFirstBuffer = true;
}

InputManagerWin::~InputManagerWin() {
    delete bufferFirst;
    bufferFirst = nullptr;
    delete bufferSecond;
    bufferSecond = nullptr;
    readBuffer = nullptr;
    writeBuffer = nullptr;
}

void InputManagerWin::Init() {}

void ME::Input::InputManagerWin::PreUpdate() {
    // Swap the read and write buffers.
    if (bReadingFirstBuffer) {
        readBuffer = bufferSecond;
        writeBuffer = bufferFirst;
    } else {
        readBuffer = bufferFirst;
        writeBuffer = bufferSecond;
    }
    bReadingFirstBuffer = !bReadingFirstBuffer;

    // Reset the write buffer for new input events.
    writeBuffer->Reset();
}

void InputManagerWin::Update(double deltaTime) {
    for (int i = 0; i < readBuffer->count; i++) {
        WinKeyEvent& event = readBuffer->buffer[i];

        // use pointer-sized type to avoid truncation on x64
        uintptr_t lp = static_cast<uintptr_t>(event.lParam);
        constexpr uintptr_t TRANSITION_MASK = (uintptr_t{1} << 31);  // 1 => transition = key released
        constexpr uintptr_t PREV_STATE_MASK = (uintptr_t{1} << 30);  // 1 => key was previously down

        bool isKeyUp = (lp & TRANSITION_MASK) != 0;
        bool wasPreviouslyDown = (lp & PREV_STATE_MASK) != 0;

        // On Key Press Down
        if (!isKeyUp) {
            KeyCode keyCode = KeyCodeMap[static_cast<uint16_t>(event.wParam)];
            if (wasPreviouslyDown) {
                // Key is being held down
                InputManager::GlobalKeyState[keyCode] = KeyState::DownWasDown;
            } else {
                // Key was just pressed
                InputManager::GlobalKeyState[keyCode] = KeyState::DownWasUp;
            }
        }

        // On Key Released
        if (isKeyUp) {
            KeyCode keyCode = KeyCodeMap[static_cast<uint16_t>(event.wParam)];
            if (wasPreviouslyDown) {
                // Key was just released
                InputManager::GlobalKeyState[keyCode] = KeyState::UpWasDown;
            } else {
                // Key is up and was previously up
                InputManager::GlobalKeyState[keyCode] = KeyState::UpWasUp;
            }
        }
    }

    // After processing all the events, reset the read buffer.
    readBuffer->Reset();
}

void ME::Input::InputManagerWin::PostUpdate() {
    // Loop through all the keys and update their states for the next frame.
    for (auto& pair : InputManager::GlobalKeyState) {
        KeyCode key = pair.first;
        KeyState state = pair.second;

        switch (state) {
            case KeyState::DownWasUp:
                // Transition from just pressed to being held down.
                InputManager::GlobalKeyState[key] = KeyState::DownWasDown;
                break;
            case KeyState::UpWasDown:
                // Transition from just released to being up.
                InputManager::GlobalKeyState[key] = KeyState::UpWasUp;
                break;
            default:
                // No state change needed for DownWasDown and UpWasUp.
                break;
        }
    }
}

void InputManagerWin::End() {}

void InputManagerWin::HandleInput(UINT msg, WPARAM wParam, LPARAM lParam) {
    writeBuffer->Push(msg, wParam, lParam);
}

ME::Input::WinKeyEventBuffer::WinKeyEventBuffer() {
    buffer = new WinKeyEvent[MAX_KEY_EVENTS];
    count = 0;
}

ME::Input::WinKeyEventBuffer::~WinKeyEventBuffer() {
    delete[] buffer;
}

void ME::Input::WinKeyEventBuffer::Push(UINT msg, WPARAM wParam, LPARAM lParam) {
    if (count >= MAX_KEY_EVENTS) {
        ME::LogError("WinKeyEventBuffer: Key event buffer overflow.");
        return;
    }
    buffer[count] = WinKeyEvent(msg, wParam, lParam);
    ++count;
}

void ME::Input::WinKeyEventBuffer::Reset() {
    count = 0;
}

ME::Input::WinKeyEvent::WinKeyEvent(UINT msg, WPARAM wParam, LPARAM lParam)
    : msg(msg), wParam(wParam), lParam(lParam) {}

#endif  // VG_WIN
