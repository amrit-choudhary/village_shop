
#include "TimeManager.h"

TimeManager::TimeManager()
{
}

TimeManager::~TimeManager()
{
}

void TimeManager::GameStart()
{
    timeScale = 1.0f;
    deltaTime = 0.0f;
    frameCout = 0;
    gameStartTP = std::chrono::high_resolution_clock::now();
    previousFrameEndTP = std::chrono::high_resolution_clock::now();
}

void TimeManager::Update()
{
    thisFrameEndTP = std::chrono::high_resolution_clock::now();
    
    auto frameDuration = thisFrameEndTP - previousFrameEndTP;
    // Convert nanoseconds to seconds.
    deltaTime = frameDuration.count() / 1'000'000'000.0f;

    auto timeStartup = thisFrameEndTP - gameStartTP;
    // Convert nanoseconds to seconds.
    timeSinceStartup = timeStartup.count() / 1'000'000'000.0f;

    ++frameCout;
    previousFrameEndTP = thisFrameEndTP;
}

void TimeManager::GameEnd()
{
    Update();
    gameEndTP = std::chrono::high_resolution_clock::now();
}

double TimeManager::GetDeltaTime() const
{
    return deltaTime * timeScale;
}

uint64_t TimeManager::GetFrameCount() const
{
    return frameCout;
}

double TimeManager::GetTimeSinceStartup() const
{
    return timeSinceStartup;
}
