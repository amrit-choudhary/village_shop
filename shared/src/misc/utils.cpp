#include "utils.h"

#include <string>

namespace {
// Full path of the executable.
static std::string executablePath;

// Directory containing the executable.
static std::string executableDirPath;

// Path of resource folder.
static std::string resourceDirPath;
}  // namespace

using namespace ME;

uint32_t ME::HashString2uint32(const char* str) {
    uint32_t hash = 5381;
    int c;
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c;
    }
    return hash;
}

void ME::SetPaths(char* arg1, char* arg2) {
    executablePath = arg1;

    executableDirPath = std::string{executablePath};
    executableDirPath.resize(executableDirPath.length() - 19);

    resourceDirPath = std::string{executableDirPath};
    resourceDirPath += "/resources/";
}

std::string ME::GetResourcesPath() { return resourceDirPath; }
