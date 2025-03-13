/**
 * Time Management class.
 * Mainly responsible for deltaTime, timescale and fps calculations.
 */

#pragma once

#include <cstdint>
#include <chrono>

class TimeManager
{
public:
    TimeManager();
    ~TimeManager();

    // Game control functions.
    void GameStart();
    void Update();
    void GameEnd();
    
    // Getters
    double GetDeltaTime() const;
    uint64_t GetFrameCount() const;
    double GetTimeSinceStartup() const;

private:
    /** Time speed multiplier used to speedup or slow down the simulation. */
    double timeScale;
    /** Time in seconds of last frame duration. */
    double deltaTime;
    /** Game time since startup in seconds. */
    double timeSinceStartup;
    /** Number of frames since game start. */
    uint64_t frameCout;
    /** Time at start of the game. */
    std::chrono::time_point<std::chrono::high_resolution_clock> gameStartTP;
    /** Time at start of the game. */
    std::chrono::time_point<std::chrono::high_resolution_clock> gameEndTP;
    /** Time at end of previous frame. */
    std::chrono::time_point<std::chrono::high_resolution_clock> previousFrameEndTP;
    /** Time at end of previous frame. */
    std::chrono::time_point<std::chrono::high_resolution_clock> thisFrameEndTP;
    /** Game time duration. */
    std::chrono::duration<double> gameDuration;
};