#include "renderer.h"

#include <iostream>
#include <string>

std::string asciiMap = R"($@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/\|()1{}[]?-_+~<>i!lI;:,"^`'.  )";
std::string asciiMapShort = R"(@%#*+=-:. XXXXX)";

Renderer::Renderer() {}

Renderer::~Renderer() {}

void Renderer::Init() {}

void Renderer::Update(std::array<std::array<uint8_t, ME::BUFFER_Y>, ME::BUFFER_X> &frameBuffer) {
    std::printf("\x1b[2J");
    for (int y = 0; y < ME::BUFFER_Y; ++y) {
        for (int x = 0; x < ME::BUFFER_X; ++x) {
            uint8_t index = frameBuffer[x][y];
            std::putchar(asciiMapShort[index - 1]);
        }
        std::putchar('\n');
    }
    std::printf("\x1b[H");
}

void Renderer::End() {}