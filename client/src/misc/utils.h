/**
 * Global utility functions.
 */
#pragma once

#include <cstdint>

namespace ME {

/**
 * Converts a c style string to uint32_t using hashing function.
 */
uint32_t HashString2uint32(const char* str);

}  // namespace ME