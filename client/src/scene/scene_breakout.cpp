#include "scene_breakout.h"

#include <cstddef>

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
    BuildTextRenderers();

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
    instancedSpriteTransforms = new ME::Transform*[Constants::MaxInstancedSpriteTransformCount];
    instancedSpriteRenderers = new ME::SpriteRenderer*[Constants::MaxInstancedSpriteRendererCount];
    spriteInstanceData = new ME::SpriteRendererInstanceData*[Constants::MaxInstancedSpriteRendererCount];
    textTransforms = new ME::Transform*[Constants::MaxTextTransformsCount];
    textRenderers = new ME::TextRenderer*[Constants::MaxTextRendererCount];
    textInstanceData = new ME::TextRendererInstanceData*[Constants::MaxTextInstanceDataCount];

    staticColliders = new ME::ColliderAABB[Constants::MaxStaticColliderCount];
    dynamicColliders = new ME::ColliderAABB[Constants::MaxDynamicColliderCount];

    meshCount = 0;

    quadPaths[0] = "meshes/quad.obj";
    quadCount = 1;

    textureCount = 0;

    spriteTexturePaths[0] = "textures/sprites/monochrome.png";
    spriteTexturePaths[1] = "textures/font/ascii_ibm_transparent.png";
    spriteTextureCount = 2;

    shaderPaths[0] = "shaders/metal/sprite.metal";
    shaderPaths[1] = "shaders/metal/sprite_instanced.metal";
    shaderCount = 2;

    textureAtlasProperties[0] = ME::TextureAtlasProperties{17, 17, 1, 1078, 49, 22, 832, 373};
    textureAtlasProperties[1] = ME::TextureAtlasProperties{10, 10, 0, 256, 16, 16, 160, 160};
    textureAtlasPropertiesCount = 2;

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
    instancedSpriteTransformCount = gridCount;

    for (size_t iy = 0; iy < gridY; ++iy) {
        for (size_t ix = 0; ix < gridX; ++ix) {
            uint16_t i = (iy * gridX) + ix;

            float px = originX + brickWidthby2 + (static_cast<float>(ix) * (brickWidth + brickPadding));
            float py = originY + brickHeightby2 + (static_cast<float>(iy) * (brickHeight + brickPadding));

            instancedSpriteTransforms[i] = new ME::Transform();
            instancedSpriteTransforms[i]->SetPosition(px, py, 1.0f);
            instancedSpriteTransforms[i]->SetScale(brickWidth, brickHeight);
        }
    }
}

void ME::SceneBreakout::BuildInstancedSpriteRenderers() {
    ME::Level level{};
    ME::Level::Load(&level, "levels/level_1.lvl");

    instancedSpriteRendererCount = gridCount;

    ME::Random randomColor("ColorInstancedSprite", true);

    // Fill the bricks only.
    for (size_t i = 0; i < instancedSpriteRendererCount; ++i) {
        instancedSpriteRenderers[i] = new ME::SpriteRenderer(0, 0, 2, 1, ME::Color::White());

        spriteInstanceData[i] = new ME::SpriteRendererInstanceData();
        spriteInstanceData[i]->modelMatrixData = instancedSpriteTransforms[i]->GetModelMatrix().GetDataForShader();
        spriteInstanceData[i]->color = ME::Color::RandomColorPretty(randomColor);

        uint8_t ix = i % gridX;
        uint8_t iy = ((i / gridX) % gridY);
        iy = gridY - 1 - iy;
        uint16_t brickIndex = (iy * gridX) + ix;

        if (brickIndex < level.brickCount) {
            spriteInstanceData[i]->atlasIndex = 587;  // Brick.
            uint8_t colorIndex = static_cast<uint8_t>(level.bricks[brickIndex].type);
            spriteInstanceData[i]->color = colorPalette[colorIndex];

            staticColliders[staticColliderCount] = ME::ColliderAABB(i, true, true, *instancedSpriteTransforms[i]);
            ++staticColliderCount;
        } else {
            spriteInstanceData[i]->atlasIndex = 0;  // Black.
            spriteInstanceData[i]->color = ME::Color::Black();
        }
    }
}

void ME::SceneBreakout::BuildTextRenderers() {
    textRendererCount = 2;
    textTransformsCount = 2;

    // Text 1
    ME::TextRenderer* textRend =
        new ME::TextRenderer{"\x0F BREAKOUT \x0F", 0, 2, 0, colorPalette[1], 80, 80, -10, 0, 0};
    textRenderers[0] = textRend;

    textTransforms[0] = new ME::Transform();
    textTransforms[0]->SetPosition(-(textRend->GetRenderWidth() / 2.0f), 940.0f, 0.0f);
    textTransforms[0]->SetScale(textRend->width, textRend->height);

    for (uint32_t i = 0; i < textRend->GetCount(); ++i) {
        textInstanceData[i] = new ME::TextRendererInstanceData();

        ME::Transform transform;
        ME::Vec3 position = textTransforms[0]->GetPosition();
        position.x += (i * (textRend->width + textRend->letterSpacing));
        transform.SetPosition(position);
        transform.SetScale(textRend->width, textRend->height);
        textInstanceData[i]->modelMatrixData = transform.GetModelMatrix().GetDataForShader();

        textInstanceData[i]->color = textRend->color;
        textInstanceData[i]->atlasIndex = textRend->text[i];
    }

    textInstanceDataCount = textRend->GetCount();

    // Text 2
    ME::TextRenderer* textRend2 = new ME::TextRenderer{"Score:000", 0, 2, 0, colorPalette[0], 70, 70, -8, 0, 0};
    textRenderers[1] = textRend2;

    textTransforms[1] = new ME::Transform();
    textTransforms[1]->SetPosition(-(textRend2->GetRenderWidth() / 2.0f), 860.0f, 0.0f);
    textTransforms[1]->SetScale(textRend2->width, textRend2->height);

    uint32_t j = textInstanceDataCount;
    for (uint32_t i = 0; i < textRend2->GetCount(); ++i) {
        textInstanceData[i + j] = new ME::TextRendererInstanceData();

        ME::Transform transform;
        ME::Vec3 position = textTransforms[1]->GetPosition();
        position.x += (i * (textRend2->width + textRend2->letterSpacing));
        transform.SetPosition(position);
        transform.SetScale(textRend2->width, textRend2->height);
        textInstanceData[i + j]->modelMatrixData = transform.GetModelMatrix().GetDataForShader();

        textInstanceData[i + j]->color = textRend2->color;
        textInstanceData[i + j]->atlasIndex = textRend2->text[i];
    }
    textInstanceDataCount += textRend2->GetCount();
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
        ++instancedSpriteTransformCount;
        instancedSpriteTransforms[indices[i]] = new ME::Transform();
        instancedSpriteTransforms[indices[i]]->SetPosition(xValues[i], yValues[i], 0.0f);
        instancedSpriteTransforms[indices[i]]->SetScale(sizeXValues[i], sizeYValues[i]);

        ++instancedSpriteRendererCount;
        instancedSpriteRenderers[indices[i]] = new ME::SpriteRenderer(0, 0, 2, 1, ME::Color::White());
        spriteInstanceData[indices[i]] = new ME::SpriteRendererInstanceData();
        spriteInstanceData[indices[i]]->modelMatrixData =
            instancedSpriteTransforms[indices[i]]->GetModelMatrix().GetDataForShader();
        spriteInstanceData[indices[i]]->atlasIndex = 253;
        spriteInstanceData[indices[i]]->color = colorPalette[7];

        staticColliders[staticColliderCount] =
            ME::ColliderAABB(indices[i], true, true, *instancedSpriteTransforms[indices[i]]);
        ++staticColliderCount;
    }
}

void ME::SceneBreakout::CreatePaddle() {
    ++instancedSpriteTransformCount;
    instancedSpriteTransforms[paddleIndex] = new ME::Transform();
    instancedSpriteTransforms[paddleIndex]->SetPosition(paddleInitX, paddleInitY, 0.0f);
    instancedSpriteTransforms[paddleIndex]->SetScale(paddleSizeX, paddleSizeY);

    ++instancedSpriteRendererCount;
    instancedSpriteRenderers[paddleIndex] = new ME::SpriteRenderer(0, 0, 2, 1, ME::Color::White());
    spriteInstanceData[paddleIndex] = new ME::SpriteRendererInstanceData();
    spriteInstanceData[paddleIndex]->modelMatrixData =
        instancedSpriteTransforms[paddleIndex]->GetModelMatrix().GetDataForShader();
    spriteInstanceData[paddleIndex]->atlasIndex = 253;
    spriteInstanceData[paddleIndex]->color = colorPalette[0];

    staticColliders[staticColliderCount] =
        ME::ColliderAABB(paddleIndex, true, true, *instancedSpriteTransforms[paddleIndex]);
    ++staticColliderCount;
}

void ME::SceneBreakout::CreateBall() {
    ++instancedSpriteTransformCount;
    instancedSpriteTransforms[ballIndex] = new ME::Transform();
    instancedSpriteTransforms[ballIndex]->SetPosition(ballInitX, ballInitY, 0.0f);
    instancedSpriteTransforms[ballIndex]->SetScale(ballSize, ballSize);

    ++instancedSpriteRendererCount;
    instancedSpriteRenderers[ballIndex] = new ME::SpriteRenderer(0, 0, 2, 1, ME::Color::White());
    spriteInstanceData[ballIndex] = new ME::SpriteRendererInstanceData();
    spriteInstanceData[ballIndex]->modelMatrixData =
        instancedSpriteTransforms[ballIndex]->GetModelMatrix().GetDataForShader();
    spriteInstanceData[ballIndex]->atlasIndex = 631;
    spriteInstanceData[ballIndex]->color = colorPalette[6];

    dynamicColliders[dynamicColliderCount] =
        ME::ColliderAABB(ballIndex, true, false, *instancedSpriteTransforms[ballIndex], ballCollScaleMult);
    ++dynamicColliderCount;
}
