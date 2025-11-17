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
    None = 0x00,
    UpWasUp = 0x01,      // Key is currently up and was previously up. Default state.
    UpWasDown = 0x02,    // Key is currently up but was previously down. Key was released.
    DownWasDown = 0x04,  // Key is currently down and was previously down. Key is being held.
    DownWasUp = 0x08,    // Key is currently down but was previously up. Key was pressed.
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
    DArrow = 8,
    Space = 9,
};

/**
 * Base class for platform specific Input implementation.
 */
class PlatformInputManager {
   public:
    PlatformInputManager();
    ~PlatformInputManager();
    virtual void Init();
    virtual void PreUpdate();
    virtual void Update(double deltaTime);
    virtual void PostUpdate();
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
    void PreUpdate();
    void Update(double deltaTime);
    void PostUpdate();
    void End();

    // Check if a key is down.
    static bool GetKeyDown(ME::Input::KeyCode keyCode);

    // Check if a key is up.
    static bool GetKeyUp(ME::Input::KeyCode keyCode);

    // Check if key was released this frame.
    static bool GetKeyReleased(ME::Input::KeyCode keyCode);

    // Check if key was pressed this frame.
    static bool GetKeyPressed(ME::Input::KeyCode keyCode);

    static std::unordered_map<ME::Input::KeyCode, ME::Input::KeyState> GlobalKeyState;

    bool GetCLIInputString(std::string& input);

    PlatformInputManager* GetPlatformInputManager();

   private:
    PlatformInputManager* platformInputManager;
};

}  // namespace ME::Input