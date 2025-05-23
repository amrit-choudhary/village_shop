#include "scene.h"

#include <cmath>

#include "../../../shared/src/random/random_engine.h"

ME::Scene::Scene() {
    CreateResources();
    BuildLights();
    BuildCamera();
    BuildTransforms();
    BuildMeshRenderers();
    BuildSpriteTransforms();
    BuildSpriteRenderers();
}

ME::Scene::~Scene() {
    delete ambientLight;
    delete directionalLight;
    delete camera;
    delete spriteCamera;

    delete[] meshPaths;
    delete[] quadPaths;
    delete[] texturePaths;
    delete[] spriteTexturePaths;
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

    for (uint16_t i = 0; i < spriteTransformCount; ++i) {
        delete spriteTransforms[i];
    }
    delete[] spriteTransforms;

    for (uint16_t i = 0; i < spriteRendererCount; ++i) {
        delete spriteRenderers[i];
    }
    delete[] spriteRenderers;
}

void ME::Scene::CreateResources() {
    meshPaths = new const char*[MaxMeshCount];
    quadPaths = new const char*[MaxQuadCount];
    texturePaths = new const char*[MaxTextureCount];
    spriteTexturePaths = new const char*[MaxSpriteTextureCount];
    shaderPaths = new const char*[MaxShaderCount];
    textureSamplers = new ME::TextureSampler[MaxSamplerCount];
    transforms = new ME::Transform*[MaxTransformCount];
    meshRenderers = new ME::MeshRenderer*[MaxMeshRendererCount];
    spriteTransforms = new ME::Transform*[MaxSpriteTransformCount];
    spriteRenderers = new ME::SpriteRenderer*[MaxSpriteRendererCount];

    // TODO: Make cubes pivot at center.
    meshPaths[0] = "meshes/cube_unshared.obj";
    meshPaths[1] = "meshes/grass.obj";
    meshCount = 2;

    quadPaths[0] = "meshes/quad.obj";  // First one is default. Not loaded from obj.
    quadCount = 1;

    // Textures for 3D objects.
    texturePaths[0] = "textures/world/cobblestone.png";
    texturePaths[1] = "textures/world/dirt.png";
    texturePaths[2] = "textures/world/ice.png";
    texturePaths[3] = "textures/world/grass_block_top.png";

    texturePaths[4] = "textures/world/short_grass.png";
    texturePaths[5] = "textures/world/torchflower.png";
    texturePaths[6] = "textures/world/poppy.png";
    texturePaths[7] = "textures/world/peony_top.png";
    textureCount = 8;

    // Textures for 2D objects.
    spriteTexturePaths[0] = "textures/sprites/tileGrey_01.png";
    spriteTexturePaths[1] = "textures/sprites/monochrome.png";
    spriteTextureCount = 2;

    shaderPaths[0] = "shaders/metal/basic.metal";
    shaderPaths[1] = "shaders/metal/basic_alpha_coutout.metal";
    shaderCount = 2;

    textureSamplers[0] = ME::TextureSampler(ME::TextureFilter::Nearest, ME::TextureWrap::Repeat);
    textureSamplerCount = 1;
}

void ME::Scene::BuildLights() {
    ambientLight = new ME::Light();
    ambientLight->color = ME::Color::White();
    ambientLight->intensity = 0.04f;

    directionalLight = new ME::Light();
    directionalLight->direction = ME::Vec3(1.0f, 2.0f, -4.0f).Normalised();
    directionalLight->color = ME::Color::White();
    directionalLight->intensity = 1.0f;
}

void ME::Scene::BuildCamera() {
    camera = new ME::Camera();
    camera->position = ME::Vec3(0.0f, 4.0f, -3.0f);
    camera->viewPosition = ME::Vec3(0.0f, 0.0f, 3.0f);
    camera->projectionType = ME::ProjectionType::Perspective;
    camera->fov = 90.0f;
    camera->aspectRatio = 1.33f;

    spriteCamera = new ME::Camera();
    spriteCamera->position = ME::Vec3(0.0f, 0.0f, 0.0f);
    spriteCamera->viewPosition = ME::Vec3(0.0f, 0.0f, 1.0f);
    spriteCamera->projectionType = ME::ProjectionType::Orthographic;
    spriteCamera->orthographicSize = 1000.0f;
    spriteCamera->aspectRatio = 1.33f;
}

void ME::Scene::BuildTransforms() {
    int gridSize = 15;
    int gridCount = gridSize * gridSize;
    transformCount = gridCount * 3;
    float spacing = 1.0f;

    for (uint16_t i = 0; i < gridCount; ++i) {
        int x = i % gridSize - 7;
        int z = (i / gridSize) % gridSize;

        float px = x * spacing;
        float pz = z * spacing;

        transforms[i * 3 + 0] = new ME::Transform();
        transforms[i * 3 + 0]->SetPosition(px, -1, pz);
        transforms[i * 3 + 0]->SetScale(1.0f, 1.0f, 1.0f);

        transforms[i * 3 + 1] = new ME::Transform();
        transforms[i * 3 + 1]->SetPosition(px, 0, pz);
        transforms[i * 3 + 1]->SetScale(1.0f, 1.0f, 1.0f);

        transforms[i * 3 + 2] = new ME::Transform();
        transforms[i * 3 + 2]->SetPosition(px, 1, pz);
        transforms[i * 3 + 2]->SetScale(0.8f, 0.8f, 0.8f);
    }
}

void ME::Scene::BuildMeshRenderers() {
    meshRendererCount = 15 * 15 * 3;
    int gridSize = std::cbrt(meshRendererCount);

    ME::Random randomTex("Tex", true);
    ME::Random randomColor("Color", true);
    for (uint16_t i = 0; i < meshRendererCount; ++i) {
        uint8_t rndTexId = randomTex.NextRange(5, 7);
        ME::Color color = ME::Color::RandomColorPretty(randomColor);

        int y = transforms[i]->GetPosition().y;

        if (y > 0.5f) {
            meshRenderers[i] = new ME::MeshRenderer(1, 0, rndTexId, ME::Color::White());
        } else if (y > -0.5f) {
            meshRenderers[i] = new ME::MeshRenderer(0, 0, 1, ME::Color(0.0f, 0.6f, 0.0f));
        } else {
            meshRenderers[i] = new ME::MeshRenderer(0, 0, 0, ME::Color::White());
        }
    }
}

void ME::Scene::BuildSpriteTransforms() {
    spriteTransformCount = 10 * 10;
    int spriteSize = 20;
    int gridCount = 10;
    int gridOffset = -90;

    for (uint16_t i = 0; i < spriteTransformCount; ++i) {
        int x = i % gridCount;
        int y = (i / gridCount) % gridCount;

        float px = x * spriteSize + gridOffset;
        float py = y * spriteSize + gridOffset;

        spriteTransforms[i] = new ME::Transform();
        spriteTransforms[i]->SetPosition(px, py, 0.0f);
        spriteTransforms[i]->SetScale(spriteSize, spriteSize);
    }
}

void ME::Scene::BuildSpriteRenderers() {
    spriteRendererCount = 10 * 10;
    int gridCount = 10;

    ME::Random randomColor("ColorSprite", true);
    for (uint16_t i = 0; i < spriteRendererCount; ++i) {
        int x = i % gridCount;
        int y = (i / gridCount) % gridCount;

        ME::Color color = ME::Color::RandomColorPretty(randomColor);
        spriteRenderers[i] = new ME::SpriteRenderer(0, 0, 0, 1, color);
    }
}
