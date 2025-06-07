#include "scene.h"

#include <cmath>
#include <cstring>

#include "../../../shared/src/random/random_engine.h"
#include "../../../shared/src/random/stb_perlin.h"

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
    BuildTextRenderers();
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

    for (uint32_t i = 0; i < textTransformsCount; ++i) {
        delete textTransforms[i];
    }
    delete[] textTransforms;

    for (uint32_t i = 0; i < textRendererCount; ++i) {
        delete textRenderers[i];
    }
    delete[] textRenderers;

    for (uint32_t i = 0; i < textInstanceDataCount; ++i) {
        delete textInstanceData[i];
    }
    delete[] textInstanceData;
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
    textTransforms = new ME::Transform*[MaxTextTransformsCount];
    textRenderers = new ME::TextRenderer*[MaxTextRendererCount];
    textInstanceData = new ME::TextRendererInstanceData*[MaxTextInstanceDataCount];

    staticColliders = new ME::ColliderAABB[MaxStaticColliderCount];
    dynamicColliders = new ME::ColliderAABB[MaxDynamicColliderCount];

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
    instancedSpriteTransformCount = 64 * 64;
    int spriteSize = 20;
    int gridCount = 64;
    int gridOffsetX = -670;
    int gridOffsetY = -500;
    int padding = 0;

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
    instancedSpriteRendererCount = 64 * 64;
    int gridCount = 64;

    ME::Random randomColor("ColorInstancedSprite", true);
    ME::Random randomAtlasIndex("AtlasIndex", true);
    ME::Random randomGround("Ground", true);
    ME::Random randomPerlinSeed("PerlinSeed", true);
    uint32_t perlinSeedR = randomPerlinSeed.Next();
    uint32_t perlinSeedG = randomPerlinSeed.Next();
    uint32_t perlinSeedB = randomPerlinSeed.Next();

    for (uint32_t i = 0; i < instancedSpriteRendererCount; ++i) {
        int x = i % gridCount;
        int y = (i / gridCount) % gridCount;

        instancedSpriteRenderers[i] = new ME::SpriteRenderer(0, 0, 2, 1, ME::Color::White());

        spriteInstanceData[i] = new ME::SpriteRendererInstanceData();
        spriteInstanceData[i]->modelMatrixData = instancedSpriteTransforms[i]->GetModelMatrix().GetData();

        if (x == 0 || x == gridCount - 1 || y == 0 || y == gridCount - 1) {
            spriteInstanceData[i]->atlasIndex = 16;
            spriteInstanceData[i]->color = ME::Color{200 / 255.0f, 200 / 255.0f, 200 / 255.0f};
        } else {
            uint32_t val = randomGround.NextRange(0, 3);
            if (val < 3) {
                // Ground.
                spriteInstanceData[i]->atlasIndex = 1;
                spriteInstanceData[i]->color = ME::Color{70 / 255.0f, 24 / 255.0f, 10 / 255.0f};
            } else {
                // Tiles

                ME::Color color = ME::Color::RandomColorPretty(randomColor);
                spriteInstanceData[i]->atlasIndex = static_cast<uint16_t>(randomAtlasIndex.NextRange(0, 1078));
                spriteInstanceData[i]->color = color;
            }
        }

        float octave = 0.045f;
        float noiseR = stb_perlin_noise3_seed(x * octave, y * octave, 0, 0, 0, 0, perlinSeedR);
        float noiseG = stb_perlin_noise3_seed(x * octave, y * octave, 0, 0, 0, 0, perlinSeedG);
        float noiseB = stb_perlin_noise3_seed(x * octave, y * octave, 0, 0, 0, 0, perlinSeedB);
        noiseR = (noiseR + 1.0f) / 2.0f;
        noiseG = (noiseG + 1.0f) / 2.0f;
        noiseB = (noiseB + 1.0f) / 2.0f;
        spriteInstanceData[i]->atlasIndex = 253;
        spriteInstanceData[i]->color = ME::Color{noiseR, noiseG, noiseB};
    }
}

void ME::Scene::BuildTextRenderers() {
    textRendererCount = 3;
    textTransformsCount = 3;

    // Text 1
    ME::TextRenderer* textRend =
        new ME::TextRenderer{"\x06 VILLAGE SHOP \x06", 0, 2, 0, ME::Color::Green(), 100, 100, -10, 0, 0};
    textRenderers[0] = textRend;

    textTransforms[0] = new ME::Transform();
    textTransforms[0]->SetPosition(-(textRend->GetRenderWidth() / 2.0f), -650.0f, 0.0f);
    textTransforms[0]->SetScale(textRend->width, textRend->height);

    for (uint32_t i = 0; i < textRend->GetCount(); ++i) {
        textInstanceData[i] = new ME::TextRendererInstanceData();

        ME::Transform transform;
        ME::Vec3 position = textTransforms[0]->GetPosition();
        position.x += (i * (textRend->width + textRend->letterSpacing));
        transform.SetPosition(position);
        transform.SetScale(textRend->width, textRend->height);
        textInstanceData[i]->modelMatrixData = transform.GetModelMatrix().GetData();

        textInstanceData[i]->color = textRend->color;
        textInstanceData[i]->atlasIndex = textRend->text[i];
    }
    textInstanceDataCount = textRend->GetCount();

    // Text 2
    ME::TextRenderer* textRend2 =
        new ME::TextRenderer{"\x10 The Game! \x11", 0, 2, 0, ME::Color::Blue(), 60, 60, -8, 0, 0};
    textRenderers[1] = textRend2;

    textTransforms[1] = new ME::Transform();
    textTransforms[1]->SetPosition(-(textRend2->GetRenderWidth() / 2.0f), -770.0f, 0.0f);
    textTransforms[1]->SetScale(textRend2->width, textRend2->height);

    uint32_t j = textInstanceDataCount;
    for (uint32_t i = 0; i < textRend2->GetCount(); ++i) {
        textInstanceData[i + j] = new ME::TextRendererInstanceData();

        ME::Transform transform;
        ME::Vec3 position = textTransforms[1]->GetPosition();
        position.x += (i * (textRend2->width + textRend2->letterSpacing));
        transform.SetPosition(position);
        transform.SetScale(textRend2->width, textRend2->height);
        textInstanceData[i + j]->modelMatrixData = transform.GetModelMatrix().GetData();

        textInstanceData[i + j]->color = textRend2->color;
        textInstanceData[i + j]->atlasIndex = textRend2->text[i];
    }
    textInstanceDataCount += textRend2->GetCount();

    // Text 3
    ME::TextRenderer* textRend3 =
        new ME::TextRenderer{"Created by Amrit Choudhary", 0, 2, 0, ME::Color::Purple(), 40, 40, -4, 0, 0};
    textRenderers[2] = textRend3;

    textTransforms[2] = new ME::Transform();
    textTransforms[2]->SetPosition(-(textRend3->GetRenderWidth() / 2.0f), -880.0f, 0.0f);
    textTransforms[2]->SetScale(textRend3->width, textRend3->height);

    uint32_t k = textInstanceDataCount;
    for (uint32_t i = 0; i < textRend3->GetCount(); ++i) {
        textInstanceData[i + k] = new ME::TextRendererInstanceData();

        ME::Transform transform;
        ME::Vec3 position = textTransforms[2]->GetPosition();
        position.x += (i * (textRend3->width + textRend3->letterSpacing));
        transform.SetPosition(position);
        transform.SetScale(textRend3->width, textRend3->height);
        textInstanceData[i + k]->modelMatrixData = transform.GetModelMatrix().GetData();

        textInstanceData[i + k]->color = textRend3->color;
        textInstanceData[i + k]->atlasIndex = textRend3->text[i];
    }
    textInstanceDataCount += textRend3->GetCount();
}
