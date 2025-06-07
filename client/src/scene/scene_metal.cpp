#ifdef VG_MAC

#include "scene_metal.h"

#include "scene_breakout.h"
#include "src/misc/game_constants.h"

ME::SceneMetal::SceneMetal(MTL::Device* device, MTL::CommandQueue* cmdQueue, ME::Scene* gameScene) {
    this->device = device;
    this->cmdQueue = cmdQueue;

    scene = gameScene;
    this->ambientLight = scene->ambientLight;
    this->directionalLight = scene->directionalLight;
    this->camera = scene->camera;
    this->spriteCamera = scene->spriteCamera;

    meshes = new ME::MeshMetal*[Constants::MaxMeshCount];
    quads = new ME::QuadMetal*[Constants::MaxQuadCount];
    textures = new ME::TextureMetal*[Constants::MaxTextureCount];
    spriteTextures = new ME::TextureMetal*[Constants::MaxSpriteTextureCount];
    textureSamplerStates = new MTL::SamplerState*[Constants::MaxSamplerCount];

    this->transforms = scene->transforms;
    this->transformCount = scene->transformCount;
    this->spriteTransforms = scene->spriteTransforms;
    this->spriteTransformCount = scene->spriteTransformCount;
    this->instancedSpriteTransforms = scene->instancedSpriteTransforms;
    this->instancedSpriteTransformCount = scene->instancedSpriteTransformCount;
    this->textTransforms = scene->textTransforms;
    this->textTransformsCount = scene->textTransformsCount;

    this->meshRenderers = scene->meshRenderers;
    this->meshRendererCount = scene->meshRendererCount;
    this->spriteRenderers = scene->spriteRenderers;
    this->spriteRendererCount = scene->spriteRendererCount;
    this->instancedSpriteRenderers = scene->instancedSpriteRenderers;
    this->instancedSpriteRendererCount = scene->instancedSpriteRendererCount;
    this->spriteInstanceData = scene->spriteInstanceData;
    this->textureAtlasProperties = scene->textureAtlasProperties;
    this->textRenderers = scene->textRenderers;
    this->textRendererCount = scene->textRendererCount;
    this->textInstanceData = scene->textInstanceData;
    this->textInstanceDataCount = scene->textInstanceDataCount;

    MakeMeshes();
    MakeQuads();
    MakeTextures();
    MakeSpriteTextures();
    MakeShaders();
    MakeTextureSamplers();

    MakeSpriteInstanceBuffer();
    MakeTextInstanceBuffer();
}

ME::SceneMetal::~SceneMetal() {
    delete ambientLight;
    delete directionalLight;
    delete camera;

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
        textureSamplerStates[i]->release();
    }
    delete[] textureSamplerStates;
}

void ME::SceneMetal::MakeMeshes() {
    for (uint8_t i = 0; i < scene->meshCount; i++) {
        meshes[i] = new ME::MeshMetal(scene->meshPaths[i], device, cmdQueue);
    }
}

void ME::SceneMetal::MakeQuads() {
    for (uint8_t i = 0; i < scene->quadCount; i++) {
        quads[i] = new ME::QuadMetal(scene->quadPaths[i], device, cmdQueue);
    }
}

void ME::SceneMetal::MakeTextures() {
    for (uint8_t i = 0; i < scene->textureCount; i++) {
        textures[i] = new ME::TextureMetal(scene->texturePaths[i], device, true, cmdQueue);
    }
}

void ME::SceneMetal::MakeSpriteTextures() {
    for (uint8_t i = 0; i < scene->spriteTextureCount; i++) {
        spriteTextures[i] = new ME::TextureMetal(scene->spriteTexturePaths[i], device, false, cmdQueue);
    }
}

void ME::SceneMetal::MakeShaders() {}

void ME::SceneMetal::MakeTextureSamplers() {
    for (uint8_t i = 0; i < scene->textureSamplerCount; i++) {
        if (scene->textureSamplers[i].filter == ME::TextureFilter::Nearest) {
            textureSamplerStates[i] = ME::TextureMetal::GetSamplerStateNearest(device);
        } else {
            textureSamplerStates[i] = ME::TextureMetal::GetSamplerStateLinear(device);
        }
    }
}

void ME::SceneMetal::MakeSpriteInstanceBuffer() {
    spriteInstanceBuffer = device->newBuffer(sizeof(ME::SpriteRendererInstanceData) * instancedSpriteRendererCount,
                                             MTL::ResourceStorageModeManaged);
}

void ME::SceneMetal::MakeTextInstanceBuffer() {
    textInstanceBuffer = device->newBuffer(sizeof(ME::TextRendererInstanceData) * textInstanceDataCount,
                                           MTL::ResourceStorageModeManaged);
}

#endif
