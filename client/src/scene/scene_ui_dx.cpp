#ifdef VG_WIN

#include "scene_ui_dx.h"

#include "shared/src/misc/game_constants.h"
#include "client/src/rendering/directx/pso_dx.h"

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

void ME::SceneUIDX::Update() {
    // The dynamic UIElement/UISystem path (unlike the static SceneUIHUD Build*() path) can add
    // elements after this object was constructed — re-sync the live counts every frame so the
    // draw calls in renderer_dx.cpp see current data. The transform/renderer/instance-data
    // array pointers themselves don't need re-syncing: they alias SceneUI's own arrays, which
    // are allocated once at max capacity and never reallocated, so RebuildUISprites/
    // RebuildUIText writes are already visible through these same pointers.
    uiSpriteRendererCount = scene->uiSpriteRendererCount;
    uiSpriteInstanceDataCount = scene->uiSpriteInstanceDataCount;
    textTransformsCount = scene->textTransformsCount;
    textRendererCount = scene->textRendererCount;
}

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
    // Initialize to a big number to support dynamic ui elements after construction.
    uiSpriteInstanceBuffer = new ME::UploadBufferDX(device, false, Constants::MaxUISpriteInstanceDataCount,
                                                    sizeof(ME::UISpriteRendererInstanceData));
    uiSpriteInstanceBufferHeapIndex = descHeapManager->CreateSRVInstanceData(uiSpriteInstanceBuffer->GetResource(),
                                                                             sizeof(ME::UISpriteRendererInstanceData),
                                                                             Constants::MaxUISpriteInstanceDataCount);
}

void ME::SceneUIDX::MakeTextInstanceBuffer() {
    // Initialize to a big number to support dynamic ui elements after construction.
    textInstanceBuffer = new ME::UploadBufferDX(device, false, Constants::MaxTextInstanceDataCount,
                                                sizeof(ME::TextRendererInstanceData));
    textInstanceBufferHeapIndex = descHeapManager->CreateSRVInstanceData(
        textInstanceBuffer->GetResource(), sizeof(ME::TextRendererInstanceData), Constants::MaxTextInstanceDataCount);
}

#endif  // VG_WIN
