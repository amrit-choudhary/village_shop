#pragma once
#ifdef VG_WIN

/**
 * Windows specific packaging.
 */

#include <string>

namespace ME::Package {

bool PackageClientWin(const std::string& exePath, const std::string& buildPath);

}  // namespace ME::Package

#endif  // VG_WIN
