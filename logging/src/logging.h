#pragma once

#ifdef _WIN32
#ifdef LOGGING_EXPORTS
#define LOGGING_API __declspec(dllexport)
#else
#define LOGGING_API __declspec(dllimport)
#endif
#else
#define LOGGING_API
#endif

namespace ME {
LOGGING_API void Log(const char *input);
}
