/**
 * Main entry for the Client Package application.
 * This will run platform specific implementations.
 */

#include <iostream>
#include <string>

#include "pkg_win.h"

int main(int argc, char** argv) {
    std::cout << "Client Package Application Started." << '\n';

#ifdef VG_WIN
    std::string exePath = argv[0];
    std::string buildPath = "";
    if (!ME::Package::PackageClientWin(exePath, buildPath)) {
        std::cerr << "Failed to package Client for Windows." << '\n';
        return -1;
    }
#endif  // VG_WIN

    return 0;
}
