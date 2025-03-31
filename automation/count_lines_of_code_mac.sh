#!/bin/zsh
set -e

echo ""
echo ""
echo ""
echo "Client"
cd ../client
cloc .

echo ""
echo ""
echo ""
echo "Server"
cd ../server
cloc .

echo ""
echo ""
echo ""
echo "Shared"
cd ../shared
cloc .
