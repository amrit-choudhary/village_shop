#!/bin/zsh
set -e
echo "All Files Formatting Started!"
find .. -type f \( -name "*.h" -o -name "*.cpp" \) -exec clang-format -i -style=file {} \;
echo "Formatting completed!"