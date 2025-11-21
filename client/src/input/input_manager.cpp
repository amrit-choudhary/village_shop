#include "input_manager.h"
#ifdef VG_MAC
#include "input_manager_cli_mac.h"
#include "input_manager_mac.h"
#endif
#ifdef VG_WIN
#include "input_manager_cli_win.h"
#include "input_manager_win.h"
#endif

using namespace ME::Input;

// Global key map with key states.
std::unordered_map<KeyCode, KeyState> InputManager::GlobalKeyState = {
    // None
    {KeyCode::None, KeyState::None},

    // Control / misc
    {KeyCode::Backspace, KeyState::UpWasUp},
    {KeyCode::Tab, KeyState::UpWasUp},
    {KeyCode::Enter, KeyState::UpWasUp},
    {KeyCode::Pause, KeyState::UpWasUp},
    {KeyCode::CapsLock, KeyState::UpWasUp},
    {KeyCode::Escape, KeyState::UpWasUp},
    {KeyCode::Space, KeyState::UpWasUp},

    // Navigation
    {KeyCode::PageUp, KeyState::UpWasUp},
    {KeyCode::PageDown, KeyState::UpWasUp},
    {KeyCode::End, KeyState::UpWasUp},
    {KeyCode::Home, KeyState::UpWasUp},
    {KeyCode::LeftArrow, KeyState::UpWasUp},
    {KeyCode::UpArrow, KeyState::UpWasUp},
    {KeyCode::RightArrow, KeyState::UpWasUp},
    {KeyCode::DownArrow, KeyState::UpWasUp},
    {KeyCode::Insert, KeyState::UpWasUp},
    {KeyCode::Delete, KeyState::UpWasUp},

    // Top row numbers
    {KeyCode::Alpha0, KeyState::UpWasUp},
    {KeyCode::Alpha1, KeyState::UpWasUp},
    {KeyCode::Alpha2, KeyState::UpWasUp},
    {KeyCode::Alpha3, KeyState::UpWasUp},
    {KeyCode::Alpha4, KeyState::UpWasUp},
    {KeyCode::Alpha5, KeyState::UpWasUp},
    {KeyCode::Alpha6, KeyState::UpWasUp},
    {KeyCode::Alpha7, KeyState::UpWasUp},
    {KeyCode::Alpha8, KeyState::UpWasUp},
    {KeyCode::Alpha9, KeyState::UpWasUp},

    // Letters
    {KeyCode::A, KeyState::UpWasUp},
    {KeyCode::B, KeyState::UpWasUp},
    {KeyCode::C, KeyState::UpWasUp},
    {KeyCode::D, KeyState::UpWasUp},
    {KeyCode::E, KeyState::UpWasUp},
    {KeyCode::F, KeyState::UpWasUp},
    {KeyCode::G, KeyState::UpWasUp},
    {KeyCode::H, KeyState::UpWasUp},
    {KeyCode::I, KeyState::UpWasUp},
    {KeyCode::J, KeyState::UpWasUp},
    {KeyCode::K, KeyState::UpWasUp},
    {KeyCode::L, KeyState::UpWasUp},
    {KeyCode::M, KeyState::UpWasUp},
    {KeyCode::N, KeyState::UpWasUp},
    {KeyCode::O, KeyState::UpWasUp},
    {KeyCode::P, KeyState::UpWasUp},
    {KeyCode::Q, KeyState::UpWasUp},
    {KeyCode::R, KeyState::UpWasUp},
    {KeyCode::S, KeyState::UpWasUp},
    {KeyCode::T, KeyState::UpWasUp},
    {KeyCode::U, KeyState::UpWasUp},
    {KeyCode::V, KeyState::UpWasUp},
    {KeyCode::W, KeyState::UpWasUp},
    {KeyCode::X, KeyState::UpWasUp},
    {KeyCode::Y, KeyState::UpWasUp},
    {KeyCode::Z, KeyState::UpWasUp},

    // Numpad
    {KeyCode::NumPad0, KeyState::UpWasUp},
    {KeyCode::NumPad1, KeyState::UpWasUp},
    {KeyCode::NumPad2, KeyState::UpWasUp},
    {KeyCode::NumPad3, KeyState::UpWasUp},
    {KeyCode::NumPad4, KeyState::UpWasUp},
    {KeyCode::NumPad5, KeyState::UpWasUp},
    {KeyCode::NumPad6, KeyState::UpWasUp},
    {KeyCode::NumPad7, KeyState::UpWasUp},
    {KeyCode::NumPad8, KeyState::UpWasUp},
    {KeyCode::NumPad9, KeyState::UpWasUp},
    {KeyCode::Multiply, KeyState::UpWasUp},
    {KeyCode::Add, KeyState::UpWasUp},
    {KeyCode::Subtract, KeyState::UpWasUp},
    {KeyCode::Decimal, KeyState::UpWasUp},
    {KeyCode::Divide, KeyState::UpWasUp},

    // Function keys
    {KeyCode::F1, KeyState::UpWasUp},
    {KeyCode::F2, KeyState::UpWasUp},
    {KeyCode::F3, KeyState::UpWasUp},
    {KeyCode::F4, KeyState::UpWasUp},
    {KeyCode::F5, KeyState::UpWasUp},
    {KeyCode::F6, KeyState::UpWasUp},
    {KeyCode::F7, KeyState::UpWasUp},
    {KeyCode::F8, KeyState::UpWasUp},
    {KeyCode::F9, KeyState::UpWasUp},
    {KeyCode::F10, KeyState::UpWasUp},
    {KeyCode::F11, KeyState::UpWasUp},
    {KeyCode::F12, KeyState::UpWasUp},

    // Locks / modifiers
    {KeyCode::NumLock, KeyState::UpWasUp},
    {KeyCode::ScrollLock, KeyState::UpWasUp},
    {KeyCode::LeftShift, KeyState::UpWasUp},
    {KeyCode::RightShift, KeyState::UpWasUp},
    {KeyCode::LeftCtrl, KeyState::UpWasUp},
    {KeyCode::RightCtrl, KeyState::UpWasUp},
    {KeyCode::LeftAlt, KeyState::UpWasUp},
    {KeyCode::RightAlt, KeyState::UpWasUp},
    {KeyCode::LeftMeta, KeyState::UpWasUp},
    {KeyCode::RightMeta, KeyState::UpWasUp},

    // Punctuation / symbols
    {KeyCode::Semicolon, KeyState::UpWasUp},
    {KeyCode::Equal, KeyState::UpWasUp},
    {KeyCode::Comma, KeyState::UpWasUp},
    {KeyCode::Minus, KeyState::UpWasUp},
    {KeyCode::Period, KeyState::UpWasUp},
    {KeyCode::Slash, KeyState::UpWasUp},
    {KeyCode::Grave, KeyState::UpWasUp},
    {KeyCode::LeftBracket, KeyState::UpWasUp},
    {KeyCode::Backslash, KeyState::UpWasUp},
    {KeyCode::RightBracket, KeyState::UpWasUp},
    {KeyCode::Apostrophe, KeyState::UpWasUp},

    // Sentinel / max
    {KeyCode::MaxValue, KeyState::None}};

InputManager::InputManager() {}

InputManager::~InputManager() {}

void InputManager::Init() {
#ifdef VG_MAC
    platformInputManager = new InputManagerCLIMac();
#endif
#ifdef VG_WIN
    platformInputManager = new InputManagerWin();
#endif

    platformInputManager->Init();
}

void ME::Input::InputManager::PreUpdate() {
    platformInputManager->PreUpdate();
}

void InputManager::Update(double deltaTime) {
    platformInputManager->Update(deltaTime);
}

void ME::Input::InputManager::PostUpdate() {
    platformInputManager->PostUpdate();
}

void InputManager::End() {
    platformInputManager->End();
}

bool InputManager::GetKeyDown(KeyCode keyCode) {
    return (GlobalKeyState[keyCode] == KeyState::DownWasDown || GlobalKeyState[keyCode] == KeyState::DownWasUp);
}

bool InputManager::GetKeyUp(KeyCode keyCode) {
    return (GlobalKeyState[keyCode] == KeyState::UpWasUp || GlobalKeyState[keyCode] == KeyState::UpWasDown);
}

bool ME::Input::InputManager::GetKeyReleased(ME::Input::KeyCode keyCode) {
    return (GlobalKeyState[keyCode] == KeyState::UpWasDown);
}

bool ME::Input::InputManager::GetKeyPressed(ME::Input::KeyCode keyCode) {
    return (GlobalKeyState[keyCode] == KeyState::DownWasUp);
}

bool InputManager::GetCLIInputString(std::string& input) {
    return platformInputManager->GetCLIInputString(input);
}

PlatformInputManager* ME::Input::InputManager::GetPlatformInputManager() {
    return platformInputManager;
}

PlatformInputManager::PlatformInputManager() {}

PlatformInputManager::~PlatformInputManager() {}

void PlatformInputManager::Init() {}

void ME::Input::PlatformInputManager::PreUpdate() {}

void PlatformInputManager::Update(double deltaTime) {}

void ME::Input::PlatformInputManager::PostUpdate() {}

void PlatformInputManager::End() {}

bool PlatformInputManager::GetCLIInputString(std::string& input) {
    return false;
}
