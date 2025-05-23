#ifdef VG_MAC

#include "scene_metal.h"

ME::SceneMetal::SceneMetal(MTL::Device* device, MTL::CommandQueue* cmdQueue) {
    this->device = device;
    this->cmdQueue = cmdQueue;

    scene = new ME::Scene();
    this->ambientLight = scene->ambientLight;
    this->directionalLight = scene->directionalLight;
    this->camera = scene->camera;
    this->spriteCamera = scene->spriteCamera;

    meshes = new ME::MeshMetal*[ME::MaxMeshCount];
    quads = new ME::QuadMetal*[ME::MaxQuadCount];
    textures = new ME::TextureMetal*[ME::MaxTextureCount];
    spriteTextures = new ME::TextureMetal*[ME::MaxSpriteTextureCount];
    textureSamplerStates = new MTL::SamplerState*[ME::MaxSamplerCount];

    this->transforms = scene->transforms;
    this->transformCount = scene->transformCount;
    this->spriteTransforms = scene->spriteTransforms;
    this->spriteTransformCount = scene->spriteTransformCount;

    this->meshRenderers = scene->meshRenderers;
    this->meshRendererCount = scene->meshRendererCount;
    this->spriteRenderers = scene->spriteRenderers;
    this->spriteRendererCount = scene->spriteRendererCount;

    MakeMeshes();
    MakeQuads();
    MakeTextures();
    MakeSpriteTextures();
    MakeShaders();
    MakeTextureSamplers();
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

    delete scene;
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

#endif
