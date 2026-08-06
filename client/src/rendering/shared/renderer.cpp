#include "renderer.h"

#include <iostream>
#include <string>

#include "client/src/rendering/directx/renderer_dx.h"
#include "client/src/rendering/metal/renderer_metal.h"

ME::Renderer::Renderer() {}

ME::Renderer::~Renderer() {}

void ME::Renderer::Init() {
#ifdef VG_MAC
    platformRenderer = new ME::RendererMetal();
#elif VG_WIN
    platformRenderer = new ME::RendererDX();
#endif
    platformRenderer->Init();
}

void ME::Renderer::Update() {
    platformRenderer->Update();
}

void ME::Renderer::End() {
    platformRenderer->End();
}

void ME::Renderer::SetVsyncEnabled(bool enabled) {
    platformRenderer->SetVsyncEnabled(enabled);
}
