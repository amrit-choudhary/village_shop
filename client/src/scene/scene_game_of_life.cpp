#include "scene_game_of_life.h"

#include <cstddef>

#include "src/misc/game_constants.h"
#include "src/random/random_engine.h"

ME::SceneGameOfLife::SceneGameOfLife() {}

ME::SceneGameOfLife::~SceneGameOfLife() {}

void ME::SceneGameOfLife::Init() {
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

void ME::SceneGameOfLife::CreateResources() {
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

void ME::SceneGameOfLife::BuildLights() {
    ME::Scene::BuildLights();
}

void ME::SceneGameOfLife::BuildCamera() {
    ME::Scene::BuildCamera();
}

void ME::SceneGameOfLife::BuildTransforms() {}

void ME::SceneGameOfLife::BuildMeshRenderers() {}

void ME::SceneGameOfLife::BuildSpriteTransforms() {}

void ME::SceneGameOfLife::BuildSpriteRenderers() {}

void ME::SceneGameOfLife::BuildInstancedSpriteTransforms() {
    instancedSpriteTransformCount = static_cast<uint32_t>(gridCount);

    for (size_t iy = 0; iy < gridHeight; ++iy) {
        for (size_t ix = 0; ix < gridWidth; ++ix) {
            size_t i = (iy * gridWidth) + ix;

            float px = originX + static_cast<float>(cellSizeby2 + (ix * (cellSize + cellPadding)));
            float py = originY + static_cast<float>(cellSizeby2 + (iy * (cellSize + cellPadding)));

            instancedSpriteTransforms[i] = new ME::Transform();
            instancedSpriteTransforms[i]->SetPosition(px, py, 1.0f);
            instancedSpriteTransforms[i]->SetScale(cellSize, cellSize);
        }
    }
}

void ME::SceneGameOfLife::BuildInstancedSpriteRenderers() {
    instancedSpriteRendererCount = static_cast<uint32_t>(gridCount);

    for (size_t i = 0; i < instancedSpriteRendererCount; ++i) {
        instancedSpriteRenderers[i] = new ME::SpriteRenderer(0, 0, 2, 1, ME::Color::White());

        spriteInstanceData[i] = new ME::SpriteRendererInstanceData();
        spriteInstanceData[i]->modelMatrixData = instancedSpriteTransforms[i]->GetModelMatrix().GetDataForShader();
        spriteInstanceData[i]->atlasIndex = 253;
        spriteInstanceData[i]->color = ME::Color::Black();
    }
}

void ME::SceneGameOfLife::BuildTextRenderers() {
    textRendererCount = 1;
    textTransformsCount = 1;

    // Text 1
    ME::TextRenderer* textRend =
        new ME::TextRenderer{"\x0F GAME OF LIFE \x0F", 0, 2, 0, ME::Color::White(), 80, 80, -10, 0, 0};
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
}
