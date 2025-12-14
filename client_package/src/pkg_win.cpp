#ifdef VG_WIN

#include "pkg_win.h"

#include <iostream>

bool ME::Package::PackageClientWin(const std::string& buildPath) {
    std::cout << "Packaging Client for Windows at path: " << buildPath << '\n';

    // Packaging logic would go here.

    return true;
}

#endif  // VG_WIN
