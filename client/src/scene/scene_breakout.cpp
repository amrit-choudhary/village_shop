#include "scene_breakout.h"

#include <cstddef>

#include "client/src/utils/json_utils.h"
#include "level_breakout.h"
#include "shared/src/misc/game_constants.h"
#include "shared/src/random/random_engine.h"

ME::SceneBreakout::SceneBreakout() : Scene() {}

void ME::SceneBreakout::Init() {
    CreateResources();
    BuildLights();
    BuildCamera();
    BuildTransforms();
    BuildMeshRenderers();
    BuildSpriteTransforms();
    BuildSpriteRenderers();
    BuildInstancedSpriteTransforms();
    BuildInstancedSpriteRenderers();

    CreateWalls();
    CreatePaddle();
    CreateBall();
}

ME::SceneBreakout::~SceneBreakout() {}

void ME::SceneBreakout::CreateResources() {
    meshPaths.data = new const char*[Constants::MaxMeshCount];
    quadPaths.data = new const char*[Constants::MaxQuadCount];
    texturePaths.data = new const char*[Constants::MaxTextureCount];
    spriteTexturePaths.data = new const char*[Constants::MaxSpriteTextureCount];
    textureAtlasProperties = new ME::TextureAtlasProperties[Constants::MaxTextureAtlasPropertiesCount];
    shaderPaths.data = new const char*[Constants::MaxShaderCount];
    textureSamplers = new ME::TextureSampler[Constants::MaxSamplerCount];
    transforms.data = new ME::Transform[Constants::MaxTransformCount];
    transforms.count = 0;
    meshRenderers.data = new ME::MeshRenderer[Constants::MaxMeshRendererCount];
    meshRenderers.count = 0;
    spriteTransforms.data = new ME::Transform[Constants::MaxSpriteTransformCount];
    spriteTransforms.count = 0;
    spriteRenderers.data = new ME::SpriteRenderer[Constants::MaxSpriteRendererCount];
    spriteRenderers.count = 0;
    instancedSpriteTransforms0.data = new ME::Transform[Constants::MaxInstancedSpriteTransformCount];
    instancedSpriteTransforms0.count = 0;
    instancedSpriteRenderers0.data = new ME::SpriteRenderer[Constants::MaxInstancedSpriteRendererCount];
    instancedSpriteRenderers0.count = 0;
    spriteInstanceData0 = new ME::SpriteRendererInstanceData[Constants::MaxInstancedSpriteRendererCount];

    staticColliders.data = new ME::ColliderAABB[Constants::MaxStaticColliderCount];
    staticColliders.count = 0;
    dynamicColliders.data = new ME::ColliderAABB[Constants::MaxDynamicColliderCount];
    dynamicColliders.count = 0;

    meshPaths.count = 0;

    quadPaths[0] = "meshes/quad.obj";
    quadPaths.count = 1;

    texturePaths.count = 0;

    spriteTexturePaths[0] = "textures/sprites/monochrome.png";
    spriteTexturePaths.count = 1;

    shaderPaths[0] = "shaders/metal/sprite.metal";
    shaderPaths[1] = "shaders/metal/sprite_instanced.metal";
    shaderPaths.count = 2;

    ME::JsonUtils::LoadTextureAtlasProps("texture_data/atlas_02.json", textureAtlasProperties[0]);
    textureAtlasPropertiesCount = 1;

    textureSamplers[0] = ME::TextureSampler(ME::TextureFilter::Nearest, ME::TextureWrap::Repeat);
    textureSamplerCount = 1;
}

void ME::SceneBreakout::BuildLights() {
    ME::Scene::BuildLights();
}

void ME::SceneBreakout::BuildCamera() {
    ME::Scene::BuildCamera();
}

void ME::SceneBreakout::BuildTransforms() {}

void ME::SceneBreakout::BuildMeshRenderers() {}

void ME::SceneBreakout::BuildSpriteTransforms() {}

void ME::SceneBreakout::BuildSpriteRenderers() {}

void ME::SceneBreakout::BuildInstancedSpriteTransforms() {
    // Create Bricks Transforms.
    instancedSpriteTransforms0.count = gridCount;

    for (size_t iy = 0; iy < gridY; ++iy) {
        for (size_t ix = 0; ix < gridX; ++ix) {
            uint16_t i = (iy * gridX) + ix;

            float px = originX + brickWidthby2 + (static_cast<float>(ix) * (brickWidth + brickPadding));
            float py = originY + brickHeightby2 + (static_cast<float>(iy) * (brickHeight + brickPadding));

            instancedSpriteTransforms0[i].SetPosition(px, py, 1.0f);
            instancedSpriteTransforms0[i].SetScale(brickWidth, brickHeight);
        }
    }
}

void ME::SceneBreakout::BuildInstancedSpriteRenderers() {
    ME::Level level{};
    ME::Level::Load(&level, "levels/level_1.lvl");

    instancedSpriteRenderers0.count = gridCount;

    ME::Random randomColor("ColorInstancedSprite", true);

    // Fill the bricks only.
    for (size_t i = 0; i < instancedSpriteRenderers0.count; ++i) {
        instancedSpriteRenderers0[i] = ME::SpriteRenderer(0, 0, 1, 1, 587, ME::Color::White());

        uint8_t ix = i % gridX;
        uint8_t iy = ((i / gridX) % gridY);
        iy = gridY - 1 - iy;
        uint16_t brickIndex = (iy * gridX) + ix;

        if (brickIndex < level.brickCount) {
            uint8_t colorIndex = static_cast<uint8_t>(level.bricks[brickIndex].type);
            instancedSpriteRenderers0[i].color = colorPalette[colorIndex];
            instancedSpriteRenderers0[i].atlasIndex = 587;

            staticColliders[staticColliders.count] = ME::ColliderAABB(i, true, true, PhysicsLayer::Default,
                                                                       PhysicsLayer::All, instancedSpriteTransforms0[i]);
            ++staticColliders.count;
        } else {
            instancedSpriteRenderers0[i].atlasIndex = 0;
            instancedSpriteRenderers0[i].color = ME::Color::Black();
        }
    }
}

void ME::SceneBreakout::CreateWalls() {
    int indices[4] = {wallIndexBottom, wallIndexRight, wallIndexTop, wallIndexLeft};

    float xValues[4];
    xValues[0] = 0.0f;
    xValues[1] = originX + (gridX * brickWidth) + wallHeightby2;
    xValues[2] = 0.0f;
    xValues[3] = originX - wallHeightby2;

    float yValues[4];
    yValues[0] = originY - wallHeightby2;
    yValues[1] = originY + (gridYby2 * brickHeight);
    yValues[2] = originY + (gridY * brickHeight) + wallHeightby2;
    yValues[3] = originY + (gridYby2 * brickHeight);

    uint16_t sizeXValues[4] = {wallSizeX, wallHeight, wallSizeX, wallHeight};
    uint16_t sizeYValues[4] = {wallHeight, wallSizeY, wallHeight, wallSizeY};

    for (int i = 0; i < 4; ++i) {
        ++instancedSpriteTransforms0.count;
        instancedSpriteTransforms0[indices[i]].SetPosition(xValues[i], yValues[i], 0.0f);
        instancedSpriteTransforms0[indices[i]].SetScale(sizeXValues[i], sizeYValues[i]);

        ++instancedSpriteRenderers0.count;
        instancedSpriteRenderers0[indices[i]] = ME::SpriteRenderer(0, 0, 2, 1, 0, ME::Color::White());
        instancedSpriteRenderers0[indices[i]].atlasIndex = 253;
        instancedSpriteRenderers0[indices[i]].color = colorPalette[7];

        staticColliders[staticColliders.count] = ColliderAABB(indices[i], true, true, PhysicsLayer::Default,
                                                               PhysicsLayer::All, instancedSpriteTransforms0[indices[i]]);
        ++staticColliders.count;
    }
}

void ME::SceneBreakout::CreatePaddle() {
    ++instancedSpriteTransforms0.count;
    instancedSpriteTransforms0[paddleIndex].SetPosition(paddleInitX, paddleInitY, 0.0f);
    instancedSpriteTransforms0[paddleIndex].SetScale(paddleSizeX, paddleSizeY);

    ++instancedSpriteRenderers0.count;
    instancedSpriteRenderers0[paddleIndex] = ME::SpriteRenderer(0, 0, 2, 1, 253, ME::Color::White());
    instancedSpriteRenderers0[paddleIndex].atlasIndex = 253;
    instancedSpriteRenderers0[paddleIndex].color = colorPalette[0];

    staticColliders[staticColliders.count] = ME::ColliderAABB(
        paddleIndex, true, true, PhysicsLayer::Default, PhysicsLayer::All, instancedSpriteTransforms0[paddleIndex]);
    ++staticColliders.count;
}

void ME::SceneBreakout::CreateBall() {
    ++instancedSpriteTransforms0.count;
    instancedSpriteTransforms0[ballIndex].SetPosition(ballInitX, ballInitY, 0.0f);
    instancedSpriteTransforms0[ballIndex].SetScale(ballSize, ballSize);

    ++instancedSpriteRenderers0.count;
    instancedSpriteRenderers0[ballIndex] = ME::SpriteRenderer(0, 0, 2, 1, 631, ME::Color::White());
    instancedSpriteRenderers0[ballIndex].atlasIndex = 631;
    instancedSpriteRenderers0[ballIndex].color = colorPalette[6];

    dynamicColliders[dynamicColliders.count] =
        ME::ColliderAABB(ballIndex, true, false, PhysicsLayer::Default, PhysicsLayer::All,
                         instancedSpriteTransforms0[ballIndex], ballCollScaleMult);
    ++dynamicColliders.count;
}
