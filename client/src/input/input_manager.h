/**
 * Input related functionality.
 */

#pragma once

#include <stdint.h>

#include <string>
#include <thread>
#include <unordered_map>

namespace ME::Input {
enum class KeyState : uint8_t {
    None = 0,
    Up = 1,
    Down = 2,
};

enum class KeyCode : uint16_t {
    None = 0,
    W = 1,
    A = 2,
    S = 3,
    D = 4,
    LArrow = 5,
    RArrow = 6,
    UArrow = 7,
    DArrow = 8
};

/**
 * Base class for platform specific Input implementation.
 */
class PlatformInputManager {
   public:
    PlatformInputManager();
    ~PlatformInputManager();
    virtual void Init();
    virtual void Update(double deltaTime);
    virtual void End();
    virtual bool GetCLIInputString(std::string& input);
};

class InputManagerMac;

/**
 * Input manager.
 */
class InputManager {
   public:
    InputManager();
    ~InputManager();

    void Init();
    void Update(double deltaTime);
    void End();

    // Check if a key is down.
    static bool GetKeyDown(ME::Input::KeyCode keyCode);

    // Check if a key is up.
    static bool GetKeyUp(ME::Input::KeyCode keyCode);

    static std::unordered_map<ME::Input::KeyCode, ME::Input::KeyState> GlobalKeyState;

    bool GetCLIInputString(std::string& input);

    PlatformInputManager* GetPlatformInputManager();

   private:
    PlatformInputManager* platformInputManager;
};

}  // namespace ME::Input