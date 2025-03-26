/**
 * Global utility functions.
 */
#pragma once

#include <cstdint>
#include <string>

namespace ME {

/**
 * Converts a c style string to uint32_t using hashing function.
 */
uint32_t HashString2uint32(const char* str);

/**
 * Sets important game directory paths.
 */
void SetPaths(char* arg1, char* arg2);

std::string GetResourcesPath();

}  // namespace ME