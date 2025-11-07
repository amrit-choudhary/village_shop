#ifdef VG_WIN

#include "input_manager_win.h"

#include <src/logging.h>

#include "../misc/global_vars.h"


using namespace ME::Input;

// Native keycode to game KeyCode conversion map.
static std::unordered_map<uint16_t, KeyCode> KeyCodeMap = {
    {87, KeyCode::W},      {83, KeyCode::S},      {65, KeyCode::A},      {68, KeyCode::D},

    {37, KeyCode::LArrow}, {39, KeyCode::RArrow}, {38, KeyCode::UArrow}, {40, KeyCode::DArrow}};

InputManagerWin::InputManagerWin() {}

InputManagerWin::~InputManagerWin() {}

void InputManagerWin::Init() {}

void InputManagerWin::Update(double deltaTime) {}

void InputManagerWin::End() {}

void InputManagerWin::HandleInput(UINT msg, WPARAM wParam, LPARAM lParam) {
    ME::Log("Key event: %d", wParam);
}

#endif  // VG_WIN
