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
    {KeyCode::None, KeyState::None},      {KeyCode::W, KeyState::UpWasUp},     {KeyCode::A, KeyState::UpWasUp},
    {KeyCode::S, KeyState::UpWasUp},      {KeyCode::D, KeyState::UpWasUp},     {KeyCode::LArrow, KeyState::UpWasUp},
    {KeyCode::RArrow, KeyState::UpWasUp}, {KeyCode::DArrow, KeyState::UpWasUp}};

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
