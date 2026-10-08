#!/bin/zsh
set -e
echo "All Files Formatting Started!"
# Vendored code (any third_party folder) and build output are never reformatted.
find .. -type d \( -name third_party -o -name build \) -prune -o -type f \( -name "*.h" -o -name "*.cpp" \) -exec clang-format -i -style=file {} \;
echo "Formatting completed!"