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
    {KeyCode::None, KeyState::None}, {KeyCode::W, KeyState::Up},     {KeyCode::A, KeyState::Up},
    {KeyCode::S, KeyState::Up},      {KeyCode::D, KeyState::Up},     {KeyCode::LArrow, KeyState::Up},
    {KeyCode::RArrow, KeyState::Up}, {KeyCode::DArrow, KeyState::Up}};

InputManager::InputManager() {}

InputManager::~InputManager() {}

void InputManager::Init() {
#ifdef VG_MAC
    platformInputManager = new InputManagerCLIMac();
#endif
#ifdef VG_WIN
    platformInputManager = new InputManagerCLIWin();
#endif

    platformInputManager->Init();
}

void InputManager::Update(double deltaTime) {
    platformInputManager->Update(deltaTime);
}

void InputManager::End() {
    platformInputManager->End();
}

bool InputManager::GetKeyDown(KeyCode keyCode) {
    return (GlobalKeyState[keyCode] == KeyState::Down);
}

bool InputManager::GetKeyUp(KeyCode keyCode) {
    return (GlobalKeyState[keyCode] == KeyState::Up);
}

bool InputManager::GetCLIInputString(std::string& input) {
    return platformInputManager->GetCLIInputString(input);
}

PlatformInputManager::PlatformInputManager() {}

PlatformInputManager::~PlatformInputManager() {}

void PlatformInputManager::Init() {}

void PlatformInputManager::Update(double deltaTime) {}

void PlatformInputManager::End() {}

bool PlatformInputManager::GetCLIInputString(std::string& input) {
    return false;
}
