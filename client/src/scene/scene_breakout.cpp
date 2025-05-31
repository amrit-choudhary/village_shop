#include "scene_breakout.h"

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
    ME::Scene::BuildInstancedSpriteTransforms();
}

void ME::SceneBreakout::BuildInstancedSpriteRenderers() {
    ME::Scene::BuildInstancedSpriteRenderers();
}

void ME::SceneBreakout::BuildTextRenderers() {
    ME::Scene::BuildTextRenderers();
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
