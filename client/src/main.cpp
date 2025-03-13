#include <iostream>
#include <thread>

#include "TimeManagement/TimeManager.h"
#include "FileIO/INI/INIParser.h"
#include "src/logging.h"

namespace
{
    const uint32_t runTimeSeconds = 2;
    double initPos = 0;
    double speed = 100; // 100 m/s;
}

int main(int argc, char **argv)
{
    TVG::Log("Game Start!");

    // Init global variables.
    TimeManager timeManager;
    timeManager.GameStart();

    while (true)
    {
        timeManager.Update();

        initPos += speed * timeManager.GetDeltaTime();

        if (timeManager.GetTimeSinceStartup() > runTimeSeconds)
        {
            break;
        }
    }
    
    for (int i = 0; i < 100'000; i++)
    {
        TVG::Log("Repeat Log!");
    }

    timeManager.GameEnd();

    std::cout << "Final Pos: " << initPos << '\n';

    std::cout << "Game Run Time: " << timeManager.GetTimeSinceStartup() << '\n';
    std::cout << "Average FPS: " << timeManager.GetFrameCount() / timeManager.GetTimeSinceStartup() << '\n';
    std::cout << "Total Frames: " << timeManager.GetFrameCount() << '\n';

    TVG::Log("Game Over!");

    INIMap iniMap = Load();
    PrintINI(iniMap);

}
