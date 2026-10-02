/**
 * Virtual file system: file access by root (FileRoot) instead of raw folder paths.
 * The only place that maps a root to a folder on disk; parsers work on text already in memory.
 */
#pragma once

#include <cstddef>
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

/**
 * Size in bytes of root + relPath. Returns false if the file can't be opened.
 */
bool GetFileSize(FileRoot root, const char* relPath, size_t& outSize);

/**
 * Reads root + relPath byte-for-byte (binary mode: line endings untouched) into caller-owned buffer.
 * Returns false if the file can't be opened, can't be fully read, or is larger than capacity.
 */
bool ReadBytes(FileRoot root, const char* relPath, uint8_t* buffer, size_t capacity, size_t& outSize);

}  // namespace Vfs
}  // namespace ME
