#!/bin/zsh
set -e

$PWD/../build/client/Debug/VillageShop_Client.exe
$PWD/../build/server/Debug/VillageShop_Server.exe

echo "Test Successful!"