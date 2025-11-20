#include "scene.h"

#include <cmath>
#include <cstring>

#include "src/misc/game_constants.h"
#include "src/random/random_engine.h"
#include "src/random/stb_perlin.h"

ME::Scene::Scene() {}

void ME::Scene::Init() {
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

    delete[] staticColliders;
    delete[] dynamicColliders;

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

    for (uint32_t i = 0; i < instancedSpriteTransformCount0; ++i) {
        delete instancedSpriteTransforms0[i];
    }
    delete[] instancedSpriteTransforms0;

    for (uint32_t i = 0; i < instancedSpriteRendererCount0; ++i) {
        delete instancedSpriteRenderers0[i];
    }
    delete[] instancedSpriteRenderers0;

    for (uint32_t i = 0; i < instancedSpriteRendererCount0; ++i) {
        delete spriteInstanceData0[i];
    }
    delete[] spriteInstanceData0;

    for (uint32_t i = 0; i < instancedSpriteTransformCount1; ++i) {
        delete instancedSpriteTransforms1[i];
    }
    delete[] instancedSpriteTransforms1;

    for (uint32_t i = 0; i < instancedSpriteRendererCount1; ++i) {
        delete instancedSpriteRenderers1[i];
    }
    delete[] instancedSpriteRenderers1;

    delete[] spriteInstanceData1;
}

void ME::Scene::CreateResources() {
    meshPaths = new const char*[Constants::MaxMeshCount];
    quadPaths = new const char*[Constants::MaxQuadCount];
    texturePaths = new const char*[Constants::MaxTextureCount];
    spriteTexturePaths = new const char*[Constants::MaxSpriteTextureCount];
    textureAtlasProperties = new ME::TextureAtlasProperties[Constants::MaxTextureAtlasPropertiesCount];
    shaderPaths = new const char*[Constants::MaxShaderCount];
    textureSamplers = new ME::TextureSampler[Constants::MaxSamplerCount];
    transforms = new ME::Transform*[Constants::MaxTransformCount];
    meshRenderers = new ME::MeshRenderer*[Constants::MaxMeshRendererCount];
    spriteTransforms = new ME::Transform*[Constants::MaxSpriteTransformCount];
    spriteRenderers = new ME::SpriteRenderer*[Constants::MaxSpriteRendererCount];

    instancedSpriteTransforms0 = new ME::Transform*[Constants::MaxInstancedSpriteTransformCount];
    instancedSpriteRenderers0 = new ME::SpriteRenderer*[Constants::MaxInstancedSpriteRendererCount];
    spriteInstanceData0 = new ME::SpriteRendererInstanceData*[Constants::MaxInstancedSpriteRendererCount];

    instancedSpriteTransforms1 = new ME::Transform*[Constants::MaxInstancedSpriteTransformCount];
    instancedSpriteRenderers1 = new ME::SpriteRenderer*[Constants::MaxInstancedSpriteRendererCount];
    spriteInstanceData1 = new ME::SpriteRendererInstanceData[Constants::MaxInstancedSpriteRendererCount];

    staticColliders = new ME::ColliderAABB[Constants::MaxStaticColliderCount];
    dynamicColliders = new ME::ColliderAABB[Constants::MaxDynamicColliderCount];

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
        spriteRenderers[i] = new ME::SpriteRenderer(0, 0, 0, 1, 1, color);
    }
}

void ME::Scene::BuildInstancedSpriteTransforms() {
    instancedSpriteTransformCount0 = 64 * 64;
    int spriteSize = 20;
    int gridCount = 64;
    int gridOffsetX = -670;
    int gridOffsetY = -500;
    int padding = 0;

    for (uint32_t i = 0; i < instancedSpriteTransformCount0; ++i) {
        int x = i % gridCount;
        int y = (i / gridCount) % gridCount;

        float px = x * (spriteSize + padding) + gridOffsetX;
        float py = y * (spriteSize + padding) + gridOffsetY;

        instancedSpriteTransforms0[i] = new ME::Transform();
        instancedSpriteTransforms0[i]->SetPosition(px, py, 0.0f);
        instancedSpriteTransforms0[i]->SetScale(spriteSize, spriteSize);
    }
}

void ME::Scene::BuildInstancedSpriteRenderers() {
    instancedSpriteRendererCount0 = 64 * 64;
    int gridCount = 64;

    ME::Random randomColor("ColorInstancedSprite", true);
    ME::Random randomAtlasIndex("AtlasIndex", true);
    ME::Random randomGround("Ground", true);
    ME::Random randomPerlinSeed("PerlinSeed", true);
    uint32_t perlinSeedR = randomPerlinSeed.Next();
    uint32_t perlinSeedG = randomPerlinSeed.Next();
    uint32_t perlinSeedB = randomPerlinSeed.Next();

    for (uint32_t i = 0; i < instancedSpriteRendererCount0; ++i) {
        int x = i % gridCount;
        int y = (i / gridCount) % gridCount;

        instancedSpriteRenderers0[i] = new ME::SpriteRenderer(0, 0, 2, 1, 1, ME::Color::White());

        spriteInstanceData0[i] = new ME::SpriteRendererInstanceData();
        spriteInstanceData0[i]->modelMatrixData = instancedSpriteTransforms0[i]->GetModelMatrix().GetData();

        if (x == 0 || x == gridCount - 1 || y == 0 || y == gridCount - 1) {
            spriteInstanceData0[i]->atlasIndex = 16;
            spriteInstanceData0[i]->color = ME::Color{200 / 255.0f, 200 / 255.0f, 200 / 255.0f};
        } else {
            uint32_t val = randomGround.NextRange(0, 3);
            if (val < 3) {
                // Ground.
                spriteInstanceData0[i]->atlasIndex = 1;
                spriteInstanceData0[i]->color = ME::Color{70 / 255.0f, 24 / 255.0f, 10 / 255.0f};
            } else {
                // Tiles

                ME::Color color = ME::Color::RandomColorPretty(randomColor);
                spriteInstanceData0[i]->atlasIndex = static_cast<uint16_t>(randomAtlasIndex.NextRange(0, 1078));
                spriteInstanceData0[i]->color = color;
            }
        }

        float octave = 0.045f;
        float noiseR = stb_perlin_noise3_seed(x * octave, y * octave, 0, 0, 0, 0, perlinSeedR);
        float noiseG = stb_perlin_noise3_seed(x * octave, y * octave, 0, 0, 0, 0, perlinSeedG);
        float noiseB = stb_perlin_noise3_seed(x * octave, y * octave, 0, 0, 0, 0, perlinSeedB);
        noiseR = (noiseR + 1.0f) / 2.0f;
        noiseG = (noiseG + 1.0f) / 2.0f;
        noiseB = (noiseB + 1.0f) / 2.0f;
        spriteInstanceData0[i]->atlasIndex = 253;
        spriteInstanceData0[i]->color = ME::Color{noiseR, noiseG, noiseB};
    }
}

void ME::Scene::Update() {
    UpdateSpriteRenderers();
    UpdateInstancedSpriteRenderers();
}

void ME::Scene::UpdateSpriteRenderers() {
    // Updating transforms and atlas indices for dirty sprites.
    // In separate loops to avoid cache misses.
    for (uint32_t i = 0; i < spriteRendererCount; ++i) {
        if (!spriteRenderers[i]->bDirty) {
            continue;
        }
        // Updated any required data before rendering.
    }

    for (uint32_t i = 0; i < spriteRendererCount; ++i) {
        if (spriteRenderers[i]->bDirty) {
            spriteRenderers[i]->bDirty = false;
        }
    }
}

void ME::Scene::UpdateInstancedSpriteRenderers() {
    // Updating transforms and atlas indicesfor dirty instances.
    // In separate loops to avoid cache misses.

    for (uint32_t i = 0; i < instancedSpriteRendererCount0; ++i) {
        if (!instancedSpriteRenderers0[i]->bDirty) {
            continue;
        }
        spriteInstanceData0[i]->modelMatrixData = instancedSpriteTransforms0[i]->GetModelMatrix().GetDataForShader();
    }

    for (uint32_t i = 0; i < instancedSpriteRendererCount0; ++i) {
        if (!instancedSpriteRenderers0[i]->bDirty) {
            continue;
        }
        spriteInstanceData0[i]->atlasIndex = instancedSpriteRenderers0[i]->atlasIndex;
        spriteInstanceData0[i]->color = instancedSpriteRenderers0[i]->color;
        spriteInstanceData0[i]->flags = instancedSpriteRenderers0[i]->flags;
    }

    for (uint32_t i = 0; i < instancedSpriteRendererCount0; ++i) {
        if (instancedSpriteRenderers0[i]->bDirty) {
            instancedSpriteRenderers0[i]->bDirty = false;
        }
    }

    for (uint32_t i = 0; i < instancedSpriteRendererCount1; ++i) {
        if (!instancedSpriteRenderers1[i]->bDirty) {
            continue;
        }
        spriteInstanceData1[i].modelMatrixData = instancedSpriteTransforms1[i]->GetModelMatrix().GetDataForShader();
    }

    for (uint32_t i = 0; i < instancedSpriteRendererCount1; ++i) {
        if (!instancedSpriteRenderers1[i]->bDirty) {
            continue;
        }
        spriteInstanceData1[i].atlasIndex = instancedSpriteRenderers1[i]->atlasIndex;
        spriteInstanceData1[i].color = instancedSpriteRenderers1[i]->color;
        spriteInstanceData1[i].flags = instancedSpriteRenderers1[i]->flags;
    }

    for (uint32_t i = 0; i < instancedSpriteRendererCount1; ++i) {
        if (instancedSpriteRenderers1[i]->bDirty) {
            instancedSpriteRenderers1[i]->bDirty = false;
        }
    }
}
