/**
 * Time Management class.
 * Mainly responsible for deltaTime, timescale and fps calculations.
 * FFR = Fixed Frame Rate System.
 * This will cap FPS to a maximum possible value.
 */

#pragma once

#include <chrono>
#include <cstdint>

namespace ME::Time {
// Constants
constexpr double FPS_10 = 10.0f;
constexpr double FPS_30 = 30.0f;
constexpr double FPS_60 = 60.0f;
constexpr double FPS_120 = 120.0f;
constexpr double FPS_MAX = 1'000'000'000'000.0f;   // 1 billion
constexpr double MIN_FRAME_TIME = 1.0f / FPS_MAX;  // Minimum allowed delta time.

class TimeManager {
   public:
    TimeManager();
    ~TimeManager();

    // Game control functions.

    /** Will initialise the clocks and setup FFR.
     * Not providing any fps will set it to uncapped FPS mode.
     */
    void Init(double fixedFrameRateFPS = FPS_MAX);

    // Will return true if game should tick based on FFR.
    bool Update();

    // Will stop the timers to let it calculate average FPS.
    void End();

    // Getters
    double GetDeltaTime() const;
    uint64_t GetFrameCount() const;
    uint64_t GetNotFFRFrameCount() const;
    double GetTimeSinceStartup() const;

   private:
    /** Time speed multiplier used to speedup or slow down the simulation. */
    double timeScale;
    /** Time in seconds of last application loop duration. */
    double deltaTime;
    /** Time in seconds of last frame duration when frame rate is capped. */
    double ffrDeltaTime;
    /** Game time since startup in seconds. */
    double timeSinceStartup;
    /** Number of frames since game start. */
    uint64_t frameCout;
    /** Number of frames since game start. */
    uint64_t ffrFrameCout;
    /** Fixed Frame Rate (FFR) time step. */
    double ffrTimeStep;
    /** FFR time accumulator. */
    double ffrTimeAcc;
    /** FFR fps. */
    double ffrFps;

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
}  // namespace ME::Time
