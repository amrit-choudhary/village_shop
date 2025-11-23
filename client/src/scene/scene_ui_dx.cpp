#ifdef VG_WIN

#include "scene_ui_dx.h"

#include "../rendering/directx/pso_dx.h"
#include "src/misc/game_constants.h"

ME::SceneUIDX::SceneUIDX(ID3D12Device* device, ID3D12GraphicsCommandList* cmdList,
                         ME::DescHeapManagerDX* descHeapManager, ME::SceneUI* uiScene) {
    this->device = device;
    this->cmdList = cmdList;
    this->descHeapManager = descHeapManager;

    scene = uiScene;

    spriteTextures = new ME::TextureDX*[Constants::MaxSpriteTextureCount];
    perPassCBs = new ME::UploadBufferDX*[Constants::MaxConstantBuffersCount];
    perPassCBHeapIndices = new uint32_t[Constants::MaxConstantBuffersCount];
    textureAtlasCBs = new ME::UploadBufferDX*[Constants::MaxConstantBuffersCount];
    textureAtlasCBHeapIndices = new uint32_t[Constants::MaxConstantBuffersCount];

    textureAtlasProperties = scene->textureAtlasProperties;

    uiSpriteTransforms = scene->uiSpriteTransforms;
    uiSpriteRenderers = scene->uiSpriteRenderers;
    uiSpriteRendererCount = scene->uiSpriteRendererCount;
    uiSpriteInstanceData = scene->uiSpriteInstanceData;
    uiSpriteInstanceDataCount = scene->uiSpriteInstanceDataCount;

    textTransforms = scene->textTransforms;
    textTransformsCount = scene->textTransformsCount;
    textRenderers = scene->textRenderers;
    textRendererCount = scene->textRendererCount;
    textInstanceData = scene->textInstanceData;
    textInstanceDataCount = &scene->textInstanceDataCount;

    MakeSpriteTextures();
    MakeConstantBuffers();

    MakeUISpriteInstanceBuffer();
    MakeTextInstanceBuffer();
}

ME::SceneUIDX::~SceneUIDX() {
    for (uint8_t i = 0; i < scene->spriteTextureCount; ++i) {
        delete spriteTextures[i];
    }
    delete[] spriteTextures;

    for (uint32_t i = 0; i < perPassCBCount; ++i) {
        delete perPassCBs[i];
    }
    delete[] perPassCBs;
    delete[] perPassCBHeapIndices;

    for (uint32_t i = 0; i < textureAtlasCBCount; ++i) {
        delete textureAtlasCBs[i];
    }
    delete[] textureAtlasCBs;
    delete[] textureAtlasCBHeapIndices;

    delete uiSpriteInstanceBuffer;
    delete textInstanceBuffer;
}

void ME::SceneUIDX::Init() {}

void ME::SceneUIDX::PostInitCleanup() {
    for (uint8_t i = 0; i < scene->spriteTextureCount; ++i) {
        spriteTextures[i]->ReleaseUploadBuffers();
    }
}

void ME::SceneUIDX::Update() {}

void ME::SceneUIDX::End() {}

void ME::SceneUIDX::MakeSpriteTextures() {
    for (uint8_t i = 0; i < scene->spriteTextureCount; ++i) {
        spriteTextures[i] = new ME::TextureDX(scene->spriteTexturePaths[i], device, cmdList);
        spriteTextures[i]->CreateBuffers(device, cmdList);
        spriteTextures[i]->descHeapIndex = descHeapManager->CreateSRVTexture(spriteTextures[i]->GetTextureBuffer());
    }
}

void ME::SceneUIDX::MakeConstantBuffers() {
    perPassCBCount = 2;

    // UI Sprite Rendering.
    perPassCBs[0] = new ME::UploadBufferDX(device, true, 1, sizeof(ME::CBPerPassUISprite));
    perPassCBHeapIndices[0] = descHeapManager->CreateCBV(perPassCBs[0]->GetResource(), perPassCBs[0]->GetElementSize());

    // UI Text Rendering.
    perPassCBs[1] = new ME::UploadBufferDX(device, true, 1, sizeof(ME::CBPerPassUIText));
    perPassCBHeapIndices[1] = descHeapManager->CreateCBV(perPassCBs[1]->GetResource(), perPassCBs[1]->GetElementSize());

    // Creating texture atlas constant buffers.
    textureAtlasCBCount = scene->textureAtlasPropertiesCount;
    for (uint32_t i = 0; i < textureAtlasCBCount; ++i) {
        textureAtlasCBs[i] = new ME::UploadBufferDX(device, true, 1, sizeof(ME::TextureAtlasProperties));
        textureAtlasCBHeapIndices[i] =
            descHeapManager->CreateCBV(textureAtlasCBs[i]->GetResource(), textureAtlasCBs[i]->GetElementSize());
    }
}

void ME::SceneUIDX::MakeUISpriteInstanceBuffer() {
    if (uiSpriteRendererCount == 0) {
        return;
    }

    uiSpriteInstanceBuffer =
        new ME::UploadBufferDX(device, false, uiSpriteRendererCount, sizeof(ME::UISpriteRendererInstanceData));
    uiSpriteInstanceBufferHeapIndex = descHeapManager->CreateSRVInstanceData(
        uiSpriteInstanceBuffer->GetResource(), sizeof(ME::UISpriteRendererInstanceData), uiSpriteRendererCount);
}

void ME::SceneUIDX::MakeTextInstanceBuffer() {
    // Text overflow buffer to handle dynamic text count changes.
    // Each text renderer can have varying number of characters,
    // Preallocate double the current count to handle increases without frequent reallocations.

    uint32_t textOverflowBufferCount = *textInstanceDataCount * 2;

    if (textOverflowBufferCount == 0) {
        return;
    }

    textInstanceBuffer =
        new ME::UploadBufferDX(device, false, textOverflowBufferCount, sizeof(ME::TextRendererInstanceData));
    textInstanceBufferHeapIndex = descHeapManager->CreateSRVInstanceData(
        textInstanceBuffer->GetResource(), sizeof(ME::TextRendererInstanceData), textOverflowBufferCount);
}

#endif  // VG_WIN
