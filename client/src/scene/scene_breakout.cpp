#include "scene_breakout.h"

#include <cstddef>

#include "../../../shared/src/random/random_engine.h"

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

    CreatePaddle();
    CreateBall();
}

ME::SceneBreakout::~SceneBreakout() {}

void ME::SceneBreakout::CreateResources() {
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

    textureAtlasProperties[0] = ME::TextureAtlasProperties{16, 16, 1, 1078, 49, 22, 832, 373};
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
    instancedSpriteTransformCount = gridCount;

    for (size_t iy = 0; iy < gridY; ++iy) {
        for (size_t ix = 0; ix < gridX; ++ix) {
            uint16_t i = (iy * gridX) + ix;

            float px = originX + (static_cast<float>(ix) * (brickWidth + brickPadding));
            float py = originY + (static_cast<float>(iy) * (brickHeight + brickPadding));

            instancedSpriteTransforms[i] = new ME::Transform();
            instancedSpriteTransforms[i]->SetPosition(px, py, 0.0f);
            instancedSpriteTransforms[i]->SetScale(brickWidth, brickHeight);
        }
    }

    // Ball.
    ++instancedSpriteTransformCount;
    instancedSpriteTransforms[ballIndex] = new ME::Transform();
    instancedSpriteTransforms[ballIndex]->SetPosition(ballInitX, ballInitY, 0.0f);
    instancedSpriteTransforms[ballIndex]->SetScale(ballSize, ballSize);

    // Paddle.
    ++instancedSpriteTransformCount;
    instancedSpriteTransforms[paddleIndex] = new ME::Transform();
    instancedSpriteTransforms[paddleIndex]->SetPosition(paddleInitX, paddleInitY, 0.0f);
    instancedSpriteTransforms[paddleIndex]->SetScale(paddleSizeX, paddleSizeY);
}

void ME::SceneBreakout::BuildInstancedSpriteRenderers() {
    instancedSpriteRendererCount = gridCount;

    ME::Random randomColor("ColorInstancedSprite", true);

    // Fill the bricks only.
    for (size_t i = 0; i < instancedSpriteRendererCount; ++i) {
        instancedSpriteRenderers[i] = new ME::SpriteRenderer(0, 0, 2, 1, ME::Color::White());

        spriteInstanceData[i] = new ME::SpriteRendererInstanceData();
        spriteInstanceData[i]->modelMatrixData = instancedSpriteTransforms[i]->GetModelMatrix().GetData();
        spriteInstanceData[i]->color = ME::Color::RandomColorPretty(randomColor);

        uint8_t ix = i % gridX;
        uint8_t iy = ((i / gridX) % gridY);
        iy = gridY - 1 - iy;

        if (iy < 20) {
            spriteInstanceData[i]->atlasIndex = 587;  // Brick.
            uint8_t cy = iy / 3;
            uint8_t colorIndex = (cy % 8);
            spriteInstanceData[i]->color = colorPalette[colorIndex];
        } else {
            spriteInstanceData[i]->atlasIndex = 0;  // Black.
            spriteInstanceData[i]->color = ME::Color::Black();
        }
    }

    // Ball.
    ++instancedSpriteRendererCount;
    instancedSpriteRenderers[ballIndex] = new ME::SpriteRenderer(0, 0, 2, 1, ME::Color::White());
    spriteInstanceData[ballIndex] = new ME::SpriteRendererInstanceData();
    spriteInstanceData[ballIndex]->modelMatrixData = instancedSpriteTransforms[ballIndex]->GetModelMatrix().GetData();
    spriteInstanceData[ballIndex]->atlasIndex = 631;
    spriteInstanceData[ballIndex]->color = colorPalette[6];

    // Paddle.
    ++instancedSpriteRendererCount;
    instancedSpriteRenderers[paddleIndex] = new ME::SpriteRenderer(0, 0, 2, 1, ME::Color::White());
    spriteInstanceData[paddleIndex] = new ME::SpriteRendererInstanceData();
    spriteInstanceData[paddleIndex]->modelMatrixData =
        instancedSpriteTransforms[paddleIndex]->GetModelMatrix().GetData();
    spriteInstanceData[paddleIndex]->atlasIndex = 253;
    spriteInstanceData[paddleIndex]->color = colorPalette[7];
}

void ME::SceneBreakout::BuildTextRenderers() {
    textRendererCount = 1;
    textTransformsCount = 1;

    // Text 1
    ME::TextRenderer* textRend =
        new ME::TextRenderer{"\x0F BREAKOUT \x0F", 0, 2, 0, ME::Color(0.515f, 0.625f, 0.708f, 1.0f), 80, 80, -10, 0, 0};
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
        textInstanceData[i]->modelMatrixData = transform.GetModelMatrix().GetData();

        textInstanceData[i]->color = textRend->color;
        textInstanceData[i]->atlasIndex = textRend->text[i];
    }

    textInstanceDataCount = textRend->GetCount();

    // Text 2
    ME::TextRenderer* textRend2 = new ME::TextRenderer{"Score:00", 0, 2, 0, ME::Color::White(), 80, 80, -8, 0, 0};
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
        textInstanceData[i + j]->modelMatrixData = transform.GetModelMatrix().GetData();

        textInstanceData[i + j]->color = textRend2->color;
        textInstanceData[i + j]->atlasIndex = textRend2->text[i];
    }
    textInstanceDataCount += textRend2->GetCount();
}

void ME::SceneBreakout::CreatePaddle() {
    // // Paddle
    // paddleTransform = new ME::Transform();
    // paddleTransform->SetPosition(0.0f, -800.0f, 0.0f);
    // paddleTransform->SetScale(200.0f, 20.0f);

    // paddleRenderer = new ME::SpriteRenderer(0, 0, 0, 1, ME::Color::White());
}
void ME::SceneBreakout::CreateBall() {
    // // Ball
    // ballTransform = new ME::Transform();
    // ballTransform->SetPosition(0.0f, -780.0f, 0.0f);
    // ballTransform->SetScale(20.0f, 20.0f);

    // ballRenderer = new ME::SpriteRenderer(0, 0, 2, 1, ME::Color::White());
}
