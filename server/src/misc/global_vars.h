/**
 * Will have all the global variables of the game server.
 */
#pragma once

#include <atomic>

// Server loop flag.
extern std::atomic<bool> ServerRunning;