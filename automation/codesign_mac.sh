#!/bin/zsh
set -e

echo "Signing"
cd ../build/client

codesign --force --sign - --entitlements debug.entitlements VillageShop_Client