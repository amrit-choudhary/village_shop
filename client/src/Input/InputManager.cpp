#include "InputManager.h"
#ifdef VG_MAC
#include "InputManagerMac.h"
#endif
#ifdef VG_WIN
#include "InputManagerWin.h"
#endif

using namespace VG::Input;

// Global key map with key states.
std::unordered_map<KeyCode, KeyState> InputManager::GlobalKeyState = {
    {KeyCode::None, KeyState::None}, {KeyCode::W, KeyState::Up},     {KeyCode::A, KeyState::Up},
    {KeyCode::S, KeyState::Up},      {KeyCode::D, KeyState::Up},     {KeyCode::LArrow, KeyState::Up},
    {KeyCode::RArrow, KeyState::Up}, {KeyCode::DArrow, KeyState::Up}};

InputManager::InputManager() {}

InputManager::~InputManager() {}

void InputManager::Init() {
#ifdef VG_MAC
    platformInputManager = new InputManagerMac();
#endif
#ifdef VG_WIN
    platformInputManager = new InputManagerWin();
#endif

    platformInputManager->Init();
}

void InputManager::Update(double deltaTime) { platformInputManager->Update(deltaTime); }

void InputManager::End() { platformInputManager->End(); }

bool InputManager::GetKeyDown(KeyCode keyCode) { return (GlobalKeyState[keyCode] == KeyState::Down); }

bool InputManager::GetKeyUp(KeyCode keyCode) { return (GlobalKeyState[keyCode] == KeyState::Up); }

PlatformInputManager::PlatformInputManager() {}

PlatformInputManager::~PlatformInputManager() {}

void PlatformInputManager::Init() {}

void PlatformInputManager::Update(double deltaTime) {}

void PlatformInputManager::End() {}