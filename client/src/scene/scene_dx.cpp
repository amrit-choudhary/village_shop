#ifdef VG_WIN

#include "scene_dx.h"

#include "src/misc/game_constants.h"

ME::SceneDX::SceneDX(ID3D12Device* device, ID3D12DescriptorHeap* descriptorHeap, ID3D12GraphicsCommandList* cmdList,
                     ME::Scene* gameScene) {
    this->device = device;
    this->descriptorHeap = descriptorHeap;
    this->cmdList = cmdList;

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

    transforms = scene->transforms;
    transformCount = scene->transformCount;
    spriteTransforms = scene->spriteTransforms;
    spriteTransformCount = scene->spriteTransformCount;
    instancedSpriteTransforms = scene->instancedSpriteTransforms;
    instancedSpriteTransformCount = scene->instancedSpriteTransformCount;
    textTransforms = scene->textTransforms;
    textTransformsCount = scene->textTransformsCount;

    meshRenderers = scene->meshRenderers;
    meshRendererCount = scene->meshRendererCount;
    spriteRenderers = scene->spriteRenderers;
    spriteRendererCount = scene->spriteRendererCount;
    instancedSpriteRenderers = scene->instancedSpriteRenderers;
    instancedSpriteRendererCount = scene->instancedSpriteRendererCount;
    spriteInstanceData = scene->spriteInstanceData;
    textureAtlasProperties = scene->textureAtlasProperties;
    textRenderers = scene->textRenderers;
    textRendererCount = scene->textRendererCount;
    textInstanceData = scene->textInstanceData;
    textInstanceDataCount = scene->textInstanceDataCount;

    MakeMeshes();
    MakeQuads();
    MakeTextures();
    MakeSpriteTextures();
    MakeShaders();
    MakeTextureSamplers();

    MakeSpriteInstanceBuffer();
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
        textures[i]->CreateGPUHandle(device, descriptorHeap, i);
    }
}

void ME::SceneDX::MakeSpriteTextures() {
    for (uint8_t i = 0; i < scene->spriteTextureCount - 1; i++) {
        spriteTextures[i] = new ME::TextureDX(scene->spriteTexturePaths[i], device, cmdList);
        spriteTextures[i]->CreateBuffers(device, cmdList);
        spriteTextures[i]->CreateGPUHandle(device, descriptorHeap, 3 + i);
    }
}

void ME::SceneDX::MakeShaders() {}

void ME::SceneDX::MakeTextureSamplers() {}

void ME::SceneDX::MakeSpriteInstanceBuffer() {}

void ME::SceneDX::MakeTextInstanceBuffer() {}

#endif  // VG_WIN
