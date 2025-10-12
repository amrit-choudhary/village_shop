#include "renderer.h"

#include <iostream>
#include <string>

#include "../directx/renderer_directx.h"
#include "../metal/renderer_metal.h"

ME::Renderer::Renderer() {}

ME::Renderer::~Renderer() {}

void ME::Renderer::Init() {
#ifdef VG_MAC
    platformRenderer = new ME::RendererMetal();
#elif VG_WIN
    platformRenderer = new ME::RendererDirectX();
#endif
    platformRenderer->Init();
}

void ME::Renderer::Update() {
    platformRenderer->Update();
}

void ME::Renderer::End() {
    platformRenderer->End();
}
