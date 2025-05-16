#include "scene.h"

#include <cmath>

#include "../../../shared/src/random/random_engine.h"

ME::Scene::Scene() {
    CreateResources();
    BuildLights();
    BuildCamera();
    BuildTransforms();
    BuildMeshRenderers();
}

ME::Scene::~Scene() {
    delete ambientLight;
    delete directionalLight;
    delete camera;

    delete[] meshPaths;
    delete[] texturePaths;
    delete[] shaderPaths;
    delete[] textureSamplers;

    for (uint16_t i = 0; i < transformCount; ++i) {
        delete transforms[i];
    }
    delete[] transforms;

    for (uint16_t i = 0; i < meshRendererCount; ++i) {
        delete meshRenderers[i];
    }
    delete[] meshRenderers;
}

void ME::Scene::CreateResources() {
    meshPaths = new const char*[MaxMeshCount];
    texturePaths = new const char*[MaxTextureCount];
    shaderPaths = new const char*[MaxShaderCount];
    textureSamplers = new ME::TextureSampler[MaxSamplerCount];
    transforms = new ME::Transform*[MaxTransformCount];
    meshRenderers = new ME::MeshRenderer*[MaxMeshRendererCount];

    meshPaths[0] = "meshes/cube_unshared.obj";
    meshCount = 1;

    texturePaths[0] = "textures/world/cobblestone.png";
    texturePaths[1] = "textures/world/dirt.png";
    texturePaths[2] = "textures/world/ice.png";
    texturePaths[3] = "textures/world/grass_block_top.png";
    textureCount = 4;

    shaderPaths[0] = "shaders/metal/basic.metal";
    shaderCount = 1;

    textureSamplers[0] = ME::TextureSampler(ME::TextureFilter::Nearest, ME::TextureWrap::Repeat);
    textureSamplerCount = 1;
}

void ME::Scene::BuildLights() {
    ambientLight = new ME::Light();
    ambientLight->color = ME::Color::White();
    ambientLight->intensity = 0.04f;

    directionalLight = new ME::Light();
    directionalLight->direction = ME::Vec3(1.0f, 1.0f, -1.0f).Normalised();
    directionalLight->color = ME::Color::White();
    directionalLight->intensity = 1.0f;
}

void ME::Scene::BuildCamera() {
    camera = new ME::Camera();
    camera->position = ME::Vec3(40.0f, 40.0f, -40.0f);
    camera->viewPosition = ME::Vec3(0.0f, 0.0f, 0.0f);
}

void ME::Scene::BuildTransforms() {
    transformCount = 3000;
    int gridSize = std::cbrt(transformCount);
    float spacing = 4.5f;

    for (uint16_t i = 0; i < transformCount; ++i) {
        int x = i % gridSize;
        int y = (i / gridSize) % gridSize;
        int z = i / (gridSize * gridSize);

        float px = (x - gridSize / 2) * spacing;
        float py = (y - gridSize / 2) * spacing;
        float pz = (z - gridSize / 2) * spacing;

        transforms[i] = new ME::Transform();
        transforms[i]->SetPosition(px, py, pz);

        float angle = (i % 360) * 1.0f;
        transforms[i]->SetRotation(0.0f, 1.0f, 0.0f, angle);

        transforms[i]->SetScale(1.0f, 1.0f, 1.0f);
    }
}

void ME::Scene::BuildMeshRenderers() {
    meshRendererCount = 3000;
    ME::Random randomTex("Tex", true);
    ME::Random randomColor("Color", true);
    for (uint16_t i = 0; i < meshRendererCount; ++i) {
        uint8_t rndTexId = randomTex.NextRange(0, textureCount - 1);
        ME::Color color = ME::Color::RandomColorPretty(randomColor);
        meshRenderers[i] = new ME::MeshRenderer{0, 0, rndTexId, color};
    }
}
