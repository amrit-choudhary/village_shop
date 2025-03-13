#include <iostream>
#include <thread>

#include "TimeManagement/TimeManager.h"
#include "FileIO/INI/INIParser.h"
#include "src/logging.h"

namespace
{
    const uint32_t runTimeSeconds = 10;
    double initPos = 0;
    double speed = 100; // 100 m/s;
}

int main(int argc, char **argv)
{
    VG::Log("Game Start!");

    // Read game params from file.
    INIMap iniMap = Load();
    int fps = std::atoi(iniMap["settings"]["fps"].c_str());

    // Init global variables.
    VG::Time::TimeManager timeManager;
    timeManager.Init(fps);
    bool shouldTick = false;

    while (true)
    {
        shouldTick = timeManager.Update();

        if (shouldTick)
        {
            // VG::Log("Tick");
            initPos += speed * timeManager.GetDeltaTime();
        }

        if (timeManager.GetTimeSinceStartup() > runTimeSeconds)
        {
            break;
        }
    }

    for (int i = 0; i < 100'000; i++)
    {
        // VG::Log("Repeat Log!");
    }

    timeManager.End();

    std::cout << "Final Pos: " << initPos << '\n';

    std::cout << "Game Run Time: " << timeManager.GetTimeSinceStartup() << '\n';
    std::cout << "Average FPS: " << timeManager.GetFrameCount() / timeManager.GetTimeSinceStartup() << '\n';
    std::cout << "Total Frames: " << timeManager.GetFrameCount() << '\n';

    VG::Log("Game Over!");
}
