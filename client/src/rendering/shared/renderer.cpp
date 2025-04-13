#include "renderer.h"

#include <iostream>
#include <string>

#include "../metal/renderer_metal.h"

ME::Renderer::Renderer() {}

ME::Renderer::~Renderer() {}

void ME::Renderer::Init() {
#ifdef VG_MAC
    platformRenderer = new ME::RendererMetal();
#endif
    platformRenderer->Init();
}

void ME::Renderer::Update() {
    platformRenderer->Update();
}

void ME::Renderer::End() {
    platformRenderer->End();
}
