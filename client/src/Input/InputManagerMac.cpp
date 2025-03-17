#ifdef VG_MAC

#include <iostream>

#include "InputManagerMac.h"

using namespace VG::Input;

// Native keycode to game KeyCode conversion map.
static std::unordered_map<uint16_t, KeyCode> KeyCodeMap = {
    {13, KeyCode::W},
    {1, KeyCode::S},
    {0, KeyCode::A},
    {2, KeyCode::D},

    {123, KeyCode::LArrow},
    {124, KeyCode::RArrow},
    {126, KeyCode::UArrow},
    {125, KeyCode::DArrow}};

#pragma clang diagnostic ignored "-Wswitch"

CGEventRef InputManagerMac::InputCallback(CGEventTapProxy proxy, CGEventType type, CGEventRef event, void *refcon)
{
    CGKeyCode keyCode = (CGKeyCode)CGEventGetIntegerValueField(event, kCGKeyboardEventKeycode);

    switch (type)
    {
    case kCGEventKeyDown:
    {
        InputManager::GlobalKeyState[KeyCodeMap[keyCode]] = KeyState::Down;
        break;
    }
    case kCGEventKeyUp:
    {
        InputManager::GlobalKeyState[KeyCodeMap[keyCode]] = KeyState::Up;
        break;
    }
    }

    return event;
}

void InputManagerMac::Init()
{
    inputThread = std::thread(&InputManagerMac::StartEventLoop, this);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

void InputManagerMac::StartEventLoop()
{
    CGEventMask eventMask = (1 << kCGEventKeyDown) | (1 << kCGEventKeyUp);

    // Create the event tap
    CFMachPortRef eventTap = CGEventTapCreate(
        kCGSessionEventTap,
        kCGHeadInsertEventTap,
        CGEventTapOptions::kCGEventTapOptionDefault,
        eventMask,
        InputCallback,
        NULL);

    if (!eventTap)
    {
        std::cerr << "Failed to create event tap. Check Accessibility permissions.\n";
    }

    // Create a run loop source
    CFRunLoopSourceRef runLoopSource = CFMachPortCreateRunLoopSource(kCFAllocatorDefault, eventTap, 0);
    if (!runLoopSource)
    {
        std::cerr << "Failed to create run loop source.\n";
        CFRelease(eventTap);
    }

    // Add the source to the main run loop
    CFRunLoopAddSource(CFRunLoopGetMain(), runLoopSource, kCFRunLoopCommonModes);
    CGEventTapEnable(eventTap, true);

    CFRunLoopRun();

    // Cleanup
    CFRelease(runLoopSource);
    CFRelease(eventTap);
    std::cout << "Event loop stopped" << std::endl;
}

void InputManagerMac::Update(double deltaTime)
{
    CFRunLoopRunInMode(kCFRunLoopDefaultMode, 0.001, false);
}

void InputManagerMac::End()
{
    CFRunLoopStop(CFRunLoopGetCurrent());
    inputThread.join();
}

#endif // VG_MAC