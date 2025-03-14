#include <iostream>
#include <string>

#include "renderer.h"

std::string asciiMap = R"($@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/\|()1{}[]?-_+~<>i!lI;:,"^`'. )";

Renderer::Renderer()
{
    lineBuffer[BUFFER_X] = '\0';
}

Renderer::~Renderer()
{
}

void Renderer::Init()
{
}

void Renderer::Update(std::array<std::array<uint8_t, BUFFER_Y>, BUFFER_X> &frameBuffer)
{
    std::printf("\x1b[2J");
    for (int y = 0; y < BUFFER_Y; ++y)
    {
        for (int x = 0; x < BUFFER_X; ++x)
        {
            uint8_t index = frameBuffer[x][y];
            lineBuffer[x] = asciiMap[index];
        }
        std::printf("%s\n", lineBuffer);
    }
    std::printf("\x1b[H");
}

void Renderer::End()
{
}