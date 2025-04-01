
#include "time_manager.h"

#include <thread>

using namespace ME::Time;

// Defining constants.
namespace {
constexpr double NANO_TO_SEC = 1'000'000'000.0f;
}

TimeManager::TimeManager() {}

TimeManager::~TimeManager() {}

void TimeManager::Init(double fixedFrameRateFPS) {
    timeScale = 1.0f;
    deltaTime = 0.0f;
    frameCout = 0;
    gameStartTP = std::chrono::high_resolution_clock::now();
    previousFrameEndTP = std::chrono::high_resolution_clock::now();
    ffrFps = fixedFrameRateFPS;
    ffrTimeStep = 1.0f / ffrFps;
    ffrTimeAcc = 0.0f;
    ffrFrameCout = 0;
}

bool TimeManager::Update() {
    // TODO: Replace with exact values of sleep time.
    std::this_thread::sleep_for(std::chrono::milliseconds(2));

    thisFrameEndTP = std::chrono::high_resolution_clock::now();

    auto frameDuration = thisFrameEndTP - previousFrameEndTP;
    // Convert nanoseconds to seconds.
    deltaTime = frameDuration.count() / NANO_TO_SEC;

    auto timeStartup = thisFrameEndTP - gameStartTP;
    // Convert nanoseconds to seconds.
    timeSinceStartup = timeStartup.count() / NANO_TO_SEC;

    ++frameCout;
    previousFrameEndTP = thisFrameEndTP;

    ffrTimeAcc += deltaTime;
    // If need to tick
    if (ffrTimeAcc >= ffrTimeStep) {
        ffrDeltaTime = ffrTimeAcc;
        ffrTimeAcc = 0.0f;
        ++ffrFrameCout;
        return true;
    } else {
        return false;
    }
}

void TimeManager::End() {
    Update();
    gameEndTP = std::chrono::high_resolution_clock::now();
}

double TimeManager::GetDeltaTime() const { return ffrDeltaTime * timeScale; }

uint64_t TimeManager::GetFrameCount() const { return ffrFrameCout; }

uint64_t TimeManager::GetNotFFRFrameCount() const { return frameCout; }

double TimeManager::GetTimeSinceStartup() const { return timeSinceStartup; }
