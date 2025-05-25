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
    BuildInstancedSpriteTransforms();
    BuildInstancedSpriteRenderers();
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
    delete[] textureAtlasProperties;
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

    for (uint32_t i = 0; i < instancedSpriteTransformCount; ++i) {
        delete instancedSpriteTransforms[i];
    }
    delete[] instancedSpriteTransforms;

    for (uint32_t i = 0; i < instancedSpriteRendererCount; ++i) {
        delete instancedSpriteRenderers[i];
    }
    delete[] instancedSpriteRenderers;

    for (uint32_t i = 0; i < instancedSpriteRendererCount; ++i) {
        delete spriteInstanceData[i];
    }
    delete[] spriteInstanceData;
}

void ME::Scene::CreateResources() {
    meshPaths = new const char*[MaxMeshCount];
    quadPaths = new const char*[MaxQuadCount];
    texturePaths = new const char*[MaxTextureCount];
    spriteTexturePaths = new const char*[MaxSpriteTextureCount];
    textureAtlasProperties = new ME::TextureAtlasProperties[MaxTextureAtlasPropertiesCount];
    shaderPaths = new const char*[MaxShaderCount];
    textureSamplers = new ME::TextureSampler[MaxSamplerCount];
    transforms = new ME::Transform*[MaxTransformCount];
    meshRenderers = new ME::MeshRenderer*[MaxMeshRendererCount];
    spriteTransforms = new ME::Transform*[MaxSpriteTransformCount];
    spriteRenderers = new ME::SpriteRenderer*[MaxSpriteRendererCount];
    instancedSpriteTransforms = new ME::Transform*[MaxInstancedSpriteTransformCount];
    instancedSpriteRenderers = new ME::SpriteRenderer*[MaxInstancedSpriteRendererCount];
    spriteInstanceData = new ME::SpriteRendererInstanceData*[MaxInstancedSpriteRendererCount];

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
    spriteTexturePaths[2] = "textures/font/ascii.png";
    spriteTexturePaths[3] = "textures/font/ascii_ibm_transparent.png";
    spriteTextureCount = 4;

    shaderPaths[0] = "shaders/metal/basic.metal";
    shaderPaths[1] = "shaders/metal/basic_alpha_coutout.metal";
    shaderPaths[2] = "shaders/metal/sprite.metal";
    shaderPaths[3] = "shaders/metal/sprite_instanced.metal";
    shaderCount = 4;

    textureAtlasProperties[0] = ME::TextureAtlasProperties{16, 16, 1, 1078, 49, 22, 832, 373};
    textureAtlasProperties[1] = ME::TextureAtlasProperties{8, 8, 0, 224, 16, 14, 128, 128};
    textureAtlasProperties[2] = ME::TextureAtlasProperties{10, 10, 0, 256, 16, 16, 160, 160};
    textureAtlasPropertiesCount = 3;

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

void ME::Scene::BuildInstancedSpriteTransforms() {
    instancedSpriteTransformCount = 2 * 20;
    int spriteSize = 80;
    int gridCount = 20;
    int gridOffsetX = -600;
    int gridOffsetY = 100;
    int padding = 8;

    for (uint32_t i = 0; i < instancedSpriteTransformCount; ++i) {
        int x = i % gridCount;
        int y = (i / gridCount) % gridCount;

        float px = x * (spriteSize + padding) + gridOffsetX;
        float py = y * (spriteSize + padding) + gridOffsetY;

        instancedSpriteTransforms[i] = new ME::Transform();
        instancedSpriteTransforms[i]->SetPosition(px, py, 0.0f);
        instancedSpriteTransforms[i]->SetScale(spriteSize, spriteSize);
    }
}

void ME::Scene::BuildInstancedSpriteRenderers() {
    instancedSpriteRendererCount = 2 * 20;
    int gridCount = 20;

    ME::Random randomColor("ColorInstancedSprite", true);
    ME::Random randomAtlasIndex("AtlasIndex", true);
    for (uint32_t i = 0; i < instancedSpriteRendererCount; ++i) {
        int x = i % gridCount;
        int y = (i / gridCount) % gridCount;

        ME::Color color = ME::Color::RandomColorPretty(randomColor);
        instancedSpriteRenderers[i] = new ME::SpriteRenderer(0, 0, 1, 1, color);

        spriteInstanceData[i] = new ME::SpriteRendererInstanceData();
        spriteInstanceData[i]->modelMatrixData = instancedSpriteTransforms[i]->GetModelMatrix().GetData();
        spriteInstanceData[i]->color = color;
        // spriteInstanceData[i]->color = ME::Color::White();
        // spriteInstanceData[i]->atlasIndex = static_cast<uint16_t>(randomAtlasIndex.NextRange(0, 1078));
        // spriteInstanceData[i]->atlasIndex = static_cast<uint16_t>(randomAtlasIndex.NextRange(32, 126));
        spriteInstanceData[i]->atlasIndex = ' ';
    }

    const char* name = "AMRIT CHOUDHARY     SWATI THAKUR";
    for (int i = 0; i < std::strlen(name); ++i) {
        spriteInstanceData[i]->atlasIndex = name[i];
    }
}
