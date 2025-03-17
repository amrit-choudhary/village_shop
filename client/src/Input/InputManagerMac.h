#ifdef VG_MAC
/**
 * Input manager or Mac
 */

#pragma once


#include "InputManager.h"
#include <ApplicationServices/ApplicationServices.h>

class InputManagerMac : public PlatformInputManager
{
public:
    void Init() override;
    void Update(double deltaTime) override;
    void End() override;

private:
};

#endif // VG_MAC
