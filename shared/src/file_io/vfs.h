/**
 * Virtual file system: file access by root (FileRoot) instead of raw folder paths.
 * The only place that maps a root to a folder on disk; parsers work on text already in memory.
 */
#pragma once

#include <cstdint>
#include <string>

namespace ME {

enum class FileRoot : uint8_t {
    Resources,  // Shipped with the build, read-only.
    Dlc,        // Downloaded from the content server.
};

namespace Vfs {

/**
 * Folder for a root, with a trailing slash.
 */
std::string GetRootPath(FileRoot root);

/**
 * Reads root + relPath as text into out. Returns false if the file can't be opened.
 */
bool ReadText(FileRoot root, const char* relPath, std::string& out);

}  // namespace Vfs
}  // namespace ME
