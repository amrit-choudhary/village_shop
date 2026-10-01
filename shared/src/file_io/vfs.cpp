#include "vfs.h"

#include <fstream>
#include <sstream>

#include "shared/src/misc/utils.h"

std::string ME::Vfs::GetRootPath(FileRoot root) {
    switch (root) {
        case FileRoot::Resources:
            return Utils::GetResourcesPath();
        case FileRoot::Dlc:
            return Utils::GetDlcPath();
    }
    return Utils::GetResourcesPath();
}

bool ME::Vfs::ReadText(FileRoot root, const char* relPath, std::string& out) {
    // Text mode: on Windows "\r\n" line endings are read as "\n", which the parsers expect.
    std::ifstream file(GetRootPath(root) + relPath);
    if (!file.is_open()) {
        return false;
    }

    std::ostringstream buffer;
    buffer << file.rdbuf();
    out = buffer.str();
    return true;
}
