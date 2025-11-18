#ifdef VG_WIN

#include "scene_dx.h"

#include "../rendering/directx/pso_dx.h"
#include "src/misc/game_constants.h"

ME::SceneDX::SceneDX(ID3D12Device* device, ID3D12GraphicsCommandList* cmdList, ME::DescHeapManagerDX* descHeapManager,
                     ME::Scene* gameScene) {
    this->device = device;
    this->cmdList = cmdList;
    this->descHeapManager = descHeapManager;

    scene = gameScene;
    this->ambientLight = scene->ambientLight;
    this->directionalLight = scene->directionalLight;
    this->camera = scene->camera;
    this->spriteCamera = scene->spriteCamera;

    meshes = new ME::MeshDX*[Constants::MaxMeshCount];
    quads = new ME::QuadDX*[Constants::MaxQuadCount];
    textures = new ME::TextureDX*[Constants::MaxTextureCount];
    spriteTextures = new ME::TextureDX*[Constants::MaxSpriteTextureCount];
    // textureSamplerStates = new MTL::SamplerState*[Constants::MaxSamplerCount];
    perPassCBs = new ME::UploadBufferDX*[Constants::MaxConstantBuffersCount];
    perPassCBHeapIndices = new uint32_t[Constants::MaxConstantBuffersCount];
    perSpriteCBs = new ME::UploadBufferDX*[Constants::MaxConstantBuffersCount];
    perSpriteCBHeapIndices = new uint32_t[Constants::MaxConstantBuffersCount];
    textureAtlasCBs = new ME::UploadBufferDX*[Constants::MaxConstantBuffersCount];
    textureAtlasCBHeapIndices = new uint32_t[Constants::MaxConstantBuffersCount];
    transforms = scene->transforms;
    transformCount = scene->transformCount;
    spriteTransforms = scene->spriteTransforms;
    spriteTransformCount = scene->spriteTransformCount;
    instancedSpriteTransforms = scene->instancedSpriteTransforms;
    instancedSpriteTransformCount = scene->instancedSpriteTransformCount;
    uiSpriteTransforms = scene->uiSpriteTransforms;
    uiSpriteTransformCount = scene->uiSpriteTransformCount;
    textTransforms = scene->textTransforms;
    textTransformsCount = scene->textTransformsCount;

    meshRenderers = scene->meshRenderers;
    meshRendererCount = scene->meshRendererCount;
    spriteRenderers = scene->spriteRenderers;
    spriteRendererCount = scene->spriteRendererCount;
    instancedSpriteRenderers = scene->instancedSpriteRenderers;
    instancedSpriteRendererCount = scene->instancedSpriteRendererCount;
    spriteInstanceData = scene->spriteInstanceData;
    uiSpriteRenderers = scene->uiSpriteRenderers;
    uiSpriteRendererCount = scene->uiSpriteRendererCount;
    uiSpriteInstanceData = scene->uiSpriteInstanceData;
    uiSpriteInstanceDataCount = scene->uiSpriteInstanceDataCount;
    textRenderers = scene->textRenderers;
    textRendererCount = scene->textRendererCount;
    textInstanceData = scene->textInstanceData;
    textInstanceDataCount = scene->textInstanceDataCount;

    textureAtlasProperties = scene->textureAtlasProperties;

    MakeMeshes();
    MakeQuads();
    MakeConstantBuffers();
    MakeTextures();
    MakeSpriteTextures();
    MakeShaders();
    MakeTextureSamplers();

    MakeSpriteInstanceBuffer();
    MakeUISpriteInstanceBuffer();
    MakeTextInstanceBuffer();
}

ME::SceneDX::~SceneDX() {
    delete ambientLight;
    delete directionalLight;
    delete camera;
    delete spriteCamera;

    for (uint8_t i = 0; i < scene->meshCount; i++) {
        delete meshes[i];
    }
    delete[] meshes;

    for (uint8_t i = 0; i < scene->quadCount; i++) {
        delete quads[i];
    }
    delete[] quads;

    for (uint8_t i = 0; i < scene->textureCount; i++) {
        delete textures[i];
    }
    delete[] textures;

    for (uint8_t i = 0; i < scene->spriteTextureCount; i++) {
        delete spriteTextures[i];
    }
    delete[] spriteTextures;

    for (uint8_t i = 0; i < scene->textureSamplerCount; i++) {
        // textureSamplerStates[i]->release();
    }
    // delete[] textureSamplerStates;

    for (uint32_t i = 0; i < perPassCBCount; i++) {
        delete perPassCBs[i];
    }
    delete[] perPassCBs;
    delete[] perPassCBHeapIndices;

    for (uint32_t i = 0; i < perSpriteCBCount; i++) {
        delete perSpriteCBs[i];
    }
    delete[] perSpriteCBs;
    delete[] perSpriteCBHeapIndices;

    for (uint32_t i = 0; i < textureAtlasCBCount; i++) {
        delete textureAtlasCBs[i];
    }
    delete[] textureAtlasCBs;
    delete[] textureAtlasCBHeapIndices;
}

void ME::SceneDX::PostInitCleanup() {
    for (uint8_t i = 0; i < scene->meshCount; i++) {
        meshes[i]->ReleaseUploadBuffers();
    }

    for (uint8_t i = 0; i < scene->quadCount; i++) {
        quads[i]->ReleaseUploadBuffers();
    }

    for (uint8_t i = 0; i < scene->textureCount; i++) {
        textures[i]->ReleaseUploadBuffers();
    }

    for (uint8_t i = 0; i < scene->spriteTextureCount; i++) {
        spriteTextures[i]->ReleaseUploadBuffers();
    }
}

void ME::SceneDX::Update() {}

void ME::SceneDX::End() {}

void ME::SceneDX::MakeMeshes() {
    for (uint8_t i = 0; i < scene->meshCount; i++) {
        meshes[i] = new ME::MeshDX(scene->meshPaths[i], device, cmdList);
        meshes[i]->CreateBuffers(device, cmdList);
    }
}

void ME::SceneDX::MakeQuads() {
    for (uint8_t i = 0; i < scene->quadCount; i++) {
        quads[i] = new ME::QuadDX(scene->quadPaths[i], device, cmdList);
        quads[i]->CreateBuffers(device, cmdList);
    }
}

void ME::SceneDX::MakeTextures() {
    for (uint8_t i = 0; i < scene->textureCount; i++) {
        textures[i] = new ME::TextureDX(scene->texturePaths[i], device, cmdList);
        textures[i]->CreateBuffers(device, cmdList);
        textures[i]->descHeapIndex = descHeapManager->CreateSRVTexture(textures[i]->GetTextureBuffer());
    }
}

void ME::SceneDX::MakeSpriteTextures() {
    for (uint8_t i = 0; i < scene->spriteTextureCount; i++) {
        spriteTextures[i] = new ME::TextureDX(scene->spriteTexturePaths[i], device, cmdList);
        spriteTextures[i]->CreateBuffers(device, cmdList);
        spriteTextures[i]->descHeapIndex = descHeapManager->CreateSRVTexture(spriteTextures[i]->GetTextureBuffer());
    }
}

void ME::SceneDX::MakeShaders() {}

void ME::SceneDX::MakeTextureSamplers() {}

void ME::SceneDX::MakeConstantBuffers() {
    // Creating per-pass constant buffers.
    perPassCBCount = 3;

    // Sprite Rendering.
    perPassCBs[0] = new ME::UploadBufferDX(device, true, 1, sizeof(ME::CBPerPass));
    perPassCBHeapIndices[0] = descHeapManager->CreateCBV(perPassCBs[0]->GetResource(), perPassCBs[0]->GetElementSize());

    // UI Sprite Rendering.
    perPassCBs[1] = new ME::UploadBufferDX(device, true, 1, sizeof(ME::CBPerPassUISprite));
    perPassCBHeapIndices[1] = descHeapManager->CreateCBV(perPassCBs[1]->GetResource(), perPassCBs[1]->GetElementSize());

    // UI Text Rendering.
    perPassCBs[2] = new ME::UploadBufferDX(device, true, 1, sizeof(ME::CBPerPassUIText));
    perPassCBHeapIndices[2] = descHeapManager->CreateCBV(perPassCBs[2]->GetResource(), perPassCBs[2]->GetElementSize());

    // Creating per-sprite constant buffers.
    perSpriteCBCount = spriteRendererCount;
    for (uint32_t i = 0; i < perSpriteCBCount; ++i) {
        perSpriteCBs[i] = new ME::UploadBufferDX(device, true, 1, sizeof(ME::CBPerSprite));
        perSpriteCBHeapIndices[i] =
            descHeapManager->CreateCBV(perSpriteCBs[i]->GetResource(), perSpriteCBs[i]->GetElementSize());
    }

    // Creating texture atlas constant buffers.
    textureAtlasCBCount = scene->textureAtlasPropertiesCount;
    for (uint32_t i = 0; i < textureAtlasCBCount; ++i) {
        textureAtlasCBs[i] = new ME::UploadBufferDX(device, true, 1, sizeof(ME::TextureAtlasProperties));
        textureAtlasCBHeapIndices[i] =
            descHeapManager->CreateCBV(textureAtlasCBs[i]->GetResource(), textureAtlasCBs[i]->GetElementSize());
    }
}

void ME::SceneDX::MakeSpriteInstanceBuffer() {
    if (instancedSpriteRendererCount == 0) {
        return;
    }

    spriteInstanceBuffer =
        new ME::UploadBufferDX(device, false, instancedSpriteRendererCount, sizeof(ME::SpriteRendererInstanceData));
    spriteInstanceBufferHeapIndex = descHeapManager->CreateSRVInstanceData(
        spriteInstanceBuffer->GetResource(), sizeof(ME::SpriteRendererInstanceData), instancedSpriteRendererCount);
}

void ME::SceneDX::MakeUISpriteInstanceBuffer() {
    if (uiSpriteInstanceDataCount == 0) {
        return;
    }

    uiSpriteInstanceBuffer =
        new ME::UploadBufferDX(device, false, uiSpriteInstanceDataCount, sizeof(ME::UISpriteRendererInstanceData));
    uiSpriteInstanceBufferHeapIndex = descHeapManager->CreateSRVInstanceData(
        uiSpriteInstanceBuffer->GetResource(), sizeof(ME::UISpriteRendererInstanceData), uiSpriteInstanceDataCount);
}

void ME::SceneDX::MakeTextInstanceBuffer() {
    if (textInstanceDataCount == 0) {
        return;
    }

    textInstanceBuffer =
        new ME::UploadBufferDX(device, false, textInstanceDataCount, sizeof(ME::TextRendererInstanceData));
    textInstanceBufferHeapIndex = descHeapManager->CreateSRVInstanceData(
        textInstanceBuffer->GetResource(), sizeof(ME::TextRendererInstanceData), textInstanceDataCount);
}

#endif  // VG_WIN
