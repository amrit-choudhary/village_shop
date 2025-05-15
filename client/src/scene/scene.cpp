#include "scene.h"

#include <cmath>

ME::Scene::Scene() {
    CreateResources();
    BuildLights();
    BuildCamera();
    BuildMeshRenderers();
    BuildTransforms();
}

ME::Scene::~Scene() {
    delete ambientLight;
    delete directionalLight;
    delete camera;

    delete[] meshPaths;
    delete[] texturePaths;
    delete[] shaderPaths;
    delete[] textureSamplers;

    for (uint16_t i = 0; i < transformCount; i++) {
        delete transforms[i];
    }
    delete[] transforms;
}

void ME::Scene::CreateResources() {
    meshPaths = new const char*[MaxMeshCount];
    texturePaths = new const char*[MaxTextureCount];
    shaderPaths = new const char*[MaxShaderCount];
    textureSamplers = new ME::TextureSampler[MaxSamplerCount];
    transforms = new ME::Transform*[MaxTransformCount];

    meshPaths[0] = "meshes/cube_unshared.obj";
    meshCount = 1;

    texturePaths[0] = "textures/world/cobblestone.png";
    textureCount = 1;

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
    camera->position = ME::Vec3(0.0f, 0.0f, -50.0f);
    camera->viewPosition = ME::Vec3(0.0f, 0.0f, 1000.0f);
}

void ME::Scene::BuildMeshRenderers() {
    ME::Transform transform;
    ME::MeshRenderer meshRenderer(0, 0, 0, transform);
    // meshRenderers.push_back(meshRenderer);
}

void ME::Scene::BuildTransforms() {
    transformCount = 3000;
    int gridSize = std::cbrt(transformCount);
    float spacing = 3.0f;

    for (uint16_t i = 0; i < transformCount; i++) {
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
