#include "renderer.h"

#include <iostream>
#include <string>

#include "metal/renderer_metal.h"

ME::Renderer::Renderer() {}

ME::Renderer::~Renderer() {}

void ME::Renderer::Init() {
    platformRenderer = new ME::RendererMetal();
    platformRenderer->Init();
}

void ME::Renderer::Update() {}

void ME::Renderer::Update(std::array<std::array<uint8_t, ME::BUFFER_Y>, ME::BUFFER_X> &frameBuffer) {}

void ME::Renderer::End() {}