#include "texture.h"

#include <iostream>

#include "src/file_io/dds/dds_parser.h"
#include "src/logging.h"

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
