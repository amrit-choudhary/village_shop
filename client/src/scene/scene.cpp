#include "scene.h"

#include <cmath>
#include <cstring>

#include "client/src/debug/debug_system.h"
#include "shared/src/misc/game_constants.h"
#include "shared/src/random/random_engine.h"
#include "shared/src/random/stb_perlin.h"

ME::Scene::Scene() {}

void ME::Scene::Init() {
    CreateResources();
    BuildLights();
    BuildCamera();
    BuildTransforms();
    BuildMeshRenderers();
    BuildSpriteTransforms();
    BuildSpriteRenderers();
    BuildInstancedSpriteTransforms();
    BuildInstancedSpriteRenderers();

    ME::DebugSystem::ScreenPrintSlot(2, GetDisplayName());
}

ME::Scene::~Scene() {
    delete ambientLight;
    delete directionalLight;
    delete camera;
    delete spriteCamera;

    delete[] meshPaths;
    delete[] quadPaths;
    delete[] texturePaths;
    delete[] spriteTexturePaths;
    delete[] textureAtlasProperties;
    delete[] shaderPaths;
    delete[] textureSamplers;

    delete[] staticColliders;
    delete[] dynamicColliders;

    delete[] sfxPaths;
    delete[] musicPaths;

    delete[] transforms.data;

    delete[] meshRenderers.data;

    delete[] spriteTransforms.data;

    delete[] spriteRenderers.data;

    delete[] instancedSpriteTransforms0.data;

    delete[] instancedSpriteRenderers0.data;

    delete[] spriteInstanceData0;

    delete[] instancedSpriteTransforms1.data;

    delete[] instancedSpriteRenderers1.data;

    delete[] spriteInstanceData1;
}

void ME::Scene::CreateResources() {
    meshPaths = new const char*[Constants::MaxMeshCount];
    quadPaths = new const char*[Constants::MaxQuadCount];
    texturePaths = new const char*[Constants::MaxTextureCount];
    spriteTexturePaths = new const char*[Constants::MaxSpriteTextureCount];
    textureAtlasProperties = new ME::TextureAtlasProperties[Constants::MaxTextureAtlasPropertiesCount];
    shaderPaths = new const char*[Constants::MaxShaderCount];
    textureSamplers = new ME::TextureSampler[Constants::MaxSamplerCount];
    transforms.data = new ME::Transform[Constants::MaxTransformCount];
    transforms.count = 0;
    meshRenderers.data = new ME::MeshRenderer[Constants::MaxMeshRendererCount];
    meshRenderers.count = 0;
    spriteTransforms.data = new ME::Transform[Constants::MaxSpriteTransformCount];
    spriteTransforms.count = 0;
    spriteRenderers.data = new ME::SpriteRenderer[Constants::MaxSpriteRendererCount];
    spriteRenderers.count = 0;

    instancedSpriteTransforms0.data = new ME::Transform[Constants::MaxInstancedSpriteTransformCount];
    instancedSpriteTransforms0.count = 0;
    instancedSpriteRenderers0.data = new ME::SpriteRenderer[Constants::MaxInstancedSpriteRendererCount];
    instancedSpriteRenderers0.count = 0;
    spriteInstanceData0 = new ME::SpriteRendererInstanceData[Constants::MaxInstancedSpriteRendererCount];

    instancedSpriteTransforms1.data = new ME::Transform[Constants::MaxInstancedSpriteTransformCount];
    instancedSpriteTransforms1.count = 0;
    instancedSpriteRenderers1.data = new ME::SpriteRenderer[Constants::MaxInstancedSpriteRendererCount];
    instancedSpriteRenderers1.count = 0;
    spriteInstanceData1 = new ME::SpriteRendererInstanceData[Constants::MaxInstancedSpriteRendererCount];

    staticColliders = new ME::ColliderAABB[Constants::MaxStaticColliderCount];
    dynamicColliders = new ME::ColliderAABB[Constants::MaxDynamicColliderCount];

    sfxPaths = new const char*[Constants::MaxLoadedSFXCount];
    sfxCount = 0;
    musicPaths = new const char*[Constants::MaxLoadedMusicCount];
    musicCount = 0;

    meshCount = 0;

    quadCount = 0;

    // For 3d objects.
    textureCount = 0;

    // Textures for 2D objects.
    spriteTextureCount = 0;

    shaderCount = 0;

    textureAtlasPropertiesCount = 0;

    textureSamplerCount = 0;
}

void ME::Scene::BuildLights() {
    ambientLight = new ME::Light();
    ambientLight->color = ME::Color::White();
    ambientLight->intensity = 0.04f;

    directionalLight = new ME::Light();
    directionalLight->direction = ME::Vec3(1.0f, 2.0f, -4.0f).Normalised();
    directionalLight->color = ME::Color::White();
    directionalLight->intensity = 1.0f;
}

void ME::Scene::BuildCamera() {
    camera = new ME::Camera();
    camera->position = ME::Vec3(0.0f, 4.0f, -3.0f);
    camera->viewPosition = ME::Vec3(0.0f, 0.0f, 3.0f);
    camera->projectionType = ME::ProjectionType::Perspective;
    camera->fov = 90.0f;
    camera->aspectRatio = 1.33f;

    spriteCamera = new ME::Camera();
    spriteCamera->position = ME::Vec3(0.0f, 0.0f, 0.0f);
    spriteCamera->viewPosition = ME::Vec3(0.0f, 0.0f, 1.0f);
    spriteCamera->projectionType = ME::ProjectionType::Orthographic;
    spriteCamera->orthographicSize = 1000.0f;
    spriteCamera->aspectRatio = 1.33f;
}

void ME::Scene::BuildTransforms() {}

void ME::Scene::BuildMeshRenderers() {}

void ME::Scene::BuildSpriteTransforms() {}

void ME::Scene::BuildSpriteRenderers() {}

void ME::Scene::BuildInstancedSpriteTransforms() {}

void ME::Scene::BuildInstancedSpriteRenderers() {}

const char* ME::Scene::GetDisplayName() const {
    return "Scene";
}

void ME::Scene::Update() {
    UpdateSpriteRenderers();
    UpdateInstancedSpriteRenderers();
}

void ME::Scene::UpdateSpriteRenderers() {
    // Updating transforms and atlas indices for dirty sprites.
    // In separate loops to avoid cache misses.
    for (size_t i = 0; i < spriteRenderers.count; ++i) {
        if (!spriteRenderers[i].bDirty) {
            continue;
        }
        // Updated any required data before rendering.
    }

    for (size_t i = 0; i < spriteRenderers.count; ++i) {
        if (spriteRenderers[i].bDirty) {
            spriteRenderers[i].bDirty = false;
        }
    }
}

void ME::Scene::UpdateInstancedSpriteRenderers() {
    // Updating transforms and atlas indicesfor dirty instances.
    // In separate loops to avoid cache misses.

    for (size_t i = 0; i < instancedSpriteRenderers0.count; ++i) {
        if (!instancedSpriteRenderers0[i].bDirty) {
            continue;
        }
        spriteInstanceData0[i].modelMatrixData = instancedSpriteTransforms0[i].GetModelMatrix().GetDataForShader();
    }

    for (size_t i = 0; i < instancedSpriteRenderers0.count; ++i) {
        if (!instancedSpriteRenderers0[i].bDirty) {
            continue;
        }
        spriteInstanceData0[i].atlasIndex = instancedSpriteRenderers0[i].atlasIndex;
        spriteInstanceData0[i].color = instancedSpriteRenderers0[i].color;
        spriteInstanceData0[i].flags = instancedSpriteRenderers0[i].flags;
    }

    for (size_t i = 0; i < instancedSpriteRenderers0.count; ++i) {
        if (instancedSpriteRenderers0[i].bDirty) {
            instancedSpriteRenderers0[i].bDirty = false;
        }
    }

    for (size_t i = 0; i < instancedSpriteRenderers1.count; ++i) {
        if (!instancedSpriteRenderers1[i].bDirty) {
            continue;
        }
        spriteInstanceData1[i].modelMatrixData = instancedSpriteTransforms1[i].GetModelMatrix().GetDataForShader();
    }

    for (size_t i = 0; i < instancedSpriteRenderers1.count; ++i) {
        if (!instancedSpriteRenderers1[i].bDirty) {
            continue;
        }
        spriteInstanceData1[i].atlasIndex = instancedSpriteRenderers1[i].atlasIndex;
        spriteInstanceData1[i].color = instancedSpriteRenderers1[i].color;
        spriteInstanceData1[i].flags = instancedSpriteRenderers1[i].flags;
    }

    for (size_t i = 0; i < instancedSpriteRenderers1.count; ++i) {
        if (instancedSpriteRenderers1[i].bDirty) {
            instancedSpriteRenderers1[i].bDirty = false;
        }
    }
}
