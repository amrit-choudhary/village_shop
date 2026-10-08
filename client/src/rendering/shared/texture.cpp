#include "texture.h"

#include <iostream>

#include "logging/src/logging.h"
#include "shared/src/file_io/dds/dds_parser.h"

ME::Texture::Texture() {}

ME::Texture::Texture(const char* path) {
    Load(path);
}

ME::Texture::~Texture() {
    delete[] data;
}

void ME::Texture::Load(const char* path) {
    DDSParser::LoadDDS(path, &data, &width, &height, &bytesPerPixel, &channels);
}
