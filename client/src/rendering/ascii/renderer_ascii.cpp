#include "renderer_ascii.h"

namespace {

std::string asciiMap = R"($@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/\|()1{}[]?-_+~<>i!lI;:,"^`'.  )";
std::string asciiMapShort = R"(@%#*+=-:. XXXXX)";

}  // namespace

void ME::RendererASCII::Init() {}

void ME::RendererASCII::Update() {
    std::printf("\x1b[2J");
    for (int y = 0; y < BUFFER_Y; ++y) {
        for (int x = 0; x < BUFFER_X; ++x) {
            uint8_t index = frameBuffer[x][y];
            std::putchar(asciiMapShort[index - 1]);
        }
        std::putchar('\n');
    }
    std::printf("\x1b[H");
}

void ME::RendererASCII::End() {}
