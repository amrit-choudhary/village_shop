#!/bin/zsh

# Define source and destination directories
SOURCE_DIR="../client/resources"
DEST_DIR="../build/client/resources"

# Copy all contents from source to destination
cp -r "$SOURCE_DIR/"* "$DEST_DIR/"

echo "Resources copied from $SOURCE_DIR to $DEST_DIR"
