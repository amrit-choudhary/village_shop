/**
 * Logging.
 */

#pragma once

namespace ME {

enum class LogLevel {
    Info,
    Warning,
    Error,
    Debug
};

void Log(const char *input);

}  // namespace ME