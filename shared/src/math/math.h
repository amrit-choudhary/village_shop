/**
 * The base header for all common math libraries.
 * Also used as common header to incldue for getting math libraries.
 */

#pragma once

#include "fp_24_8.h"

namespace ME::Math {

using FP = ME::Math::FP_24_8;

// Test add function.
template <typename T>
T add(T a, T b) {
    return a + b;
}
}  // namespace ME::Math