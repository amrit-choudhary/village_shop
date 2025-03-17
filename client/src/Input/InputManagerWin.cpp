#ifdef VG_WIN

#include "InputManagerWin.h"

using namespace VG::Input;

// Native keycode to game KeyCode conversion map.
static std::unordered_map<uint16_t, KeyCode> KeyCodeMap = {
    {87, KeyCode::W},
    {83, KeyCode::S},
    {65, KeyCode::A},
    {68, KeyCode::D},

    {37, KeyCode::LArrow},
    {39, KeyCode::RArrow},
    {38, KeyCode::UArrow},
    {40, KeyCode::DArrow}};

/**
 * Input Callback linked to the windows event system.
 *
 */
void InputManagerWin::InputCallback(HANDLE hInput)
{
    INPUT_RECORD inputRecords[128];
    DWORD eventsRead;
    int counter = 0;

    // Run forever.
    while (true)
    {
        ReadConsoleInput(hInput, inputRecords, 128, &eventsRead);

        for (int i = 0; i < eventsRead; ++i)
        {
            if (inputRecords[i].EventType == KEY_EVENT)
            {
                if (inputRecords[i].Event.KeyEvent.bKeyDown)
                {
                    InputManager::GlobalKeyState[KeyCodeMap[inputRecords[i].Event.KeyEvent.wVirtualKeyCode]] = KeyState::Down;
                }
                else
                {
                    InputManager::GlobalKeyState[KeyCodeMap[inputRecords[i].Event.KeyEvent.wVirtualKeyCode]] = KeyState::Up;
                }
            }
        }
    }
}

InputManagerWin::InputManagerWin()
{
}

InputManagerWin::~InputManagerWin()
{
}

void InputManagerWin::Init()
{
    HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
    inputThread = std::thread(&InputManagerWin::InputCallback, this, hInput);
}

void InputManagerWin::Update(double deltaTime)
{
}

void InputManagerWin::End()
{
    inputThread.join();
}

#endif // VG_WIN