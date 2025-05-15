#include "scene.h"

ME::Scene::Scene() {
    CreateResources();
    BuildLights();
    BuildCamera();
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
}

void ME::Scene::CreateResources() {
    meshPaths = new const char*[MaxMeshCount];
    texturePaths = new const char*[MaxTextureCount];
    shaderPaths = new const char*[MaxShaderCount];
    textureSamplers = new ME::TextureSampler[MaxSamplerCount];

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
    camera->position = ME::Vec3(0.0f, 0.0f, -10.0f);
    camera->viewPosition = ME::Vec3(0.0f, 0.0f, 100.0f);
}

void ME::Scene::BuildMeshRenderers() {
    ME::Transform transform;
    ME::MeshRenderer meshRenderer(0, 0, 0, transform);
    // meshRenderers.push_back(meshRenderer);
}
