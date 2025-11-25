#include "scene_breakout.h"

#include <cstddef>

#include "../utils/json_utils.h"
#include "level_breakout.h"
#include "src/misc/game_constants.h"
#include "src/random/random_engine.h"

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
    spriteInstanceData0 = new ME::SpriteRendererInstanceData[Constants::MaxInstancedSpriteRendererCount];

    staticColliders = new ME::ColliderAABB[Constants::MaxStaticColliderCount];
    dynamicColliders = new ME::ColliderAABB[Constants::MaxDynamicColliderCount];

    meshCount = 0;

    quadPaths[0] = "meshes/quad.obj";
    quadCount = 1;

    textureCount = 0;

    spriteTexturePaths[0] = "textures/sprites/monochrome.png";
    spriteTextureCount = 1;

    shaderPaths[0] = "shaders/metal/sprite.metal";
    shaderPaths[1] = "shaders/metal/sprite_instanced.metal";
    shaderCount = 2;

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
    instancedSpriteTransformCount0 = gridCount;

    for (size_t iy = 0; iy < gridY; ++iy) {
        for (size_t ix = 0; ix < gridX; ++ix) {
            uint16_t i = (iy * gridX) + ix;

            float px = originX + brickWidthby2 + (static_cast<float>(ix) * (brickWidth + brickPadding));
            float py = originY + brickHeightby2 + (static_cast<float>(iy) * (brickHeight + brickPadding));

            instancedSpriteTransforms0[i] = new ME::Transform();
            instancedSpriteTransforms0[i]->SetPosition(px, py, 1.0f);
            instancedSpriteTransforms0[i]->SetScale(brickWidth, brickHeight);
        }
    }
}

void ME::SceneBreakout::BuildInstancedSpriteRenderers() {
    ME::Level level{};
    ME::Level::Load(&level, "levels/level_1.lvl");

    instancedSpriteRendererCount0 = gridCount;

    ME::Random randomColor("ColorInstancedSprite", true);

    // Fill the bricks only.
    for (size_t i = 0; i < instancedSpriteRendererCount0; ++i) {
        instancedSpriteRenderers0[i] = new ME::SpriteRenderer(0, 0, 1, 1, 587, ME::Color::White());

        uint8_t ix = i % gridX;
        uint8_t iy = ((i / gridX) % gridY);
        iy = gridY - 1 - iy;
        uint16_t brickIndex = (iy * gridX) + ix;

        if (brickIndex < level.brickCount) {
            uint8_t colorIndex = static_cast<uint8_t>(level.bricks[brickIndex].type);
            instancedSpriteRenderers0[i]->color = colorPalette[colorIndex];
            instancedSpriteRenderers0[i]->atlasIndex = 587;

            staticColliders[staticColliderCount] = ME::ColliderAABB(i, true, true, PhysicsLayer::Default,
                                                                    PhysicsLayer::All, *instancedSpriteTransforms0[i]);
            ++staticColliderCount;
        } else {
            instancedSpriteRenderers0[i]->atlasIndex = 0;
            instancedSpriteRenderers0[i]->color = ME::Color::Black();
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
        ++instancedSpriteTransformCount0;
        instancedSpriteTransforms0[indices[i]] = new ME::Transform();
        instancedSpriteTransforms0[indices[i]]->SetPosition(xValues[i], yValues[i], 0.0f);
        instancedSpriteTransforms0[indices[i]]->SetScale(sizeXValues[i], sizeYValues[i]);

        ++instancedSpriteRendererCount0;
        instancedSpriteRenderers0[indices[i]] = new ME::SpriteRenderer(0, 0, 2, 1, 0, ME::Color::White());
        instancedSpriteRenderers0[indices[i]]->atlasIndex = 253;
        instancedSpriteRenderers0[indices[i]]->color = colorPalette[7];

        staticColliders[staticColliderCount] = ColliderAABB(indices[i], true, true, PhysicsLayer::Default,
                                                            PhysicsLayer::All, *instancedSpriteTransforms0[indices[i]]);
        ++staticColliderCount;
    }
}

void ME::SceneBreakout::CreatePaddle() {
    ++instancedSpriteTransformCount0;
    instancedSpriteTransforms0[paddleIndex] = new ME::Transform();
    instancedSpriteTransforms0[paddleIndex]->SetPosition(paddleInitX, paddleInitY, 0.0f);
    instancedSpriteTransforms0[paddleIndex]->SetScale(paddleSizeX, paddleSizeY);

    ++instancedSpriteRendererCount0;
    instancedSpriteRenderers0[paddleIndex] = new ME::SpriteRenderer(0, 0, 2, 1, 253, ME::Color::White());
    instancedSpriteRenderers0[paddleIndex]->atlasIndex = 253;
    instancedSpriteRenderers0[paddleIndex]->color = colorPalette[0];

    staticColliders[staticColliderCount] = ME::ColliderAABB(
        paddleIndex, true, true, PhysicsLayer::Default, PhysicsLayer::All, *instancedSpriteTransforms0[paddleIndex]);
    ++staticColliderCount;
}

void ME::SceneBreakout::CreateBall() {
    ++instancedSpriteTransformCount0;
    instancedSpriteTransforms0[ballIndex] = new ME::Transform();
    instancedSpriteTransforms0[ballIndex]->SetPosition(ballInitX, ballInitY, 0.0f);
    instancedSpriteTransforms0[ballIndex]->SetScale(ballSize, ballSize);

    ++instancedSpriteRendererCount0;
    instancedSpriteRenderers0[ballIndex] = new ME::SpriteRenderer(0, 0, 2, 1, 631, ME::Color::White());
    instancedSpriteRenderers0[ballIndex]->atlasIndex = 631;
    instancedSpriteRenderers0[ballIndex]->color = colorPalette[6];

    dynamicColliders[dynamicColliderCount] =
        ME::ColliderAABB(ballIndex, true, false, PhysicsLayer::Default, PhysicsLayer::All,
                         *instancedSpriteTransforms0[ballIndex], ballCollScaleMult);
    ++dynamicColliderCount;
}
