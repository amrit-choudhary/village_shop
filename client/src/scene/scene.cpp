#include "scene.h"

#include <src/logging.h>

#include <cmath>
#include <cstring>

#include "src/misc/game_constants.h"
#include "src/random/random_engine.h"
#include "src/random/stb_perlin.h"

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

    for (uint16_t i = 0; i < transformCount; ++i) {
        delete transforms[i];
    }
    delete[] transforms;

    for (uint16_t i = 0; i < meshRendererCount; ++i) {
        delete meshRenderers[i];
    }
    delete[] meshRenderers;

    for (uint16_t i = 0; i < spriteTransformCount; ++i) {
        delete spriteTransforms[i];
    }
    delete[] spriteTransforms;

    for (uint16_t i = 0; i < spriteRendererCount; ++i) {
        delete spriteRenderers[i];
    }
    delete[] spriteRenderers;

    for (uint32_t i = 0; i < instancedSpriteTransformCount0; ++i) {
        delete instancedSpriteTransforms0[i];
    }
    delete[] instancedSpriteTransforms0;

    for (uint32_t i = 0; i < instancedSpriteRendererCount0; ++i) {
        delete instancedSpriteRenderers0[i];
    }
    delete[] instancedSpriteRenderers0;

    delete[] spriteInstanceData0;

    for (uint32_t i = 0; i < instancedSpriteTransformCount1; ++i) {
        delete instancedSpriteTransforms1[i];
    }
    delete[] instancedSpriteTransforms1;

    for (uint32_t i = 0; i < instancedSpriteRendererCount1; ++i) {
        delete instancedSpriteRenderers1[i];
    }
    delete[] instancedSpriteRenderers1;

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
    transforms = new ME::Transform*[Constants::MaxTransformCount];
    meshRenderers = new ME::MeshRenderer*[Constants::MaxMeshRendererCount];
    spriteTransforms = new ME::Transform*[Constants::MaxSpriteTransformCount];
    spriteRenderers = new ME::SpriteRenderer*[Constants::MaxSpriteRendererCount];

    instancedSpriteTransforms0 = new ME::Transform*[Constants::MaxInstancedSpriteTransformCount];
    instancedSpriteRenderers0 = new ME::SpriteRenderer*[Constants::MaxInstancedSpriteRendererCount];
    spriteInstanceData0 = new ME::SpriteRendererInstanceData[Constants::MaxInstancedSpriteRendererCount];

    instancedSpriteTransforms1 = new ME::Transform*[Constants::MaxInstancedSpriteTransformCount];
    instancedSpriteRenderers1 = new ME::SpriteRenderer*[Constants::MaxInstancedSpriteRendererCount];
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

void ME::Scene::AddSpriteTransform(ME::Vec3 position, ME::Vec3 scale) {
    if (spriteTransformCount >= Constants::MaxSpriteTransformCount) {
        ME::LogError("Scene::AddSpriteTransform: Exceeded max sprite transform count.");
        return;
    }

    ME::Transform* transform = new ME::Transform();
    transform->SetPosition(position);
    transform->SetScale(scale);
    spriteTransforms[spriteTransformCount] = transform;
    ++spriteTransformCount;
}

void ME::Scene::AddSpriteRenderer(ME::SpriteRenderer* spriteRenderer) {
    if (spriteRendererCount >= Constants::MaxSpriteRendererCount) {
        ME::LogError("Scene::AddSpriteRenderer: Exceeded max sprite renderer count.");
        return;
    }

    spriteRenderers[spriteRendererCount] = spriteRenderer;
    ++spriteRendererCount;
}

void ME::Scene::AddInstancedSpriteTransform(ME::Vec3 position, ME::Vec3 scale, uint8_t buffer) {
    if (buffer == 0) {
        if (instancedSpriteTransformCount0 >= Constants::MaxInstancedSpriteTransformCount) {
            ME::LogError(
                "Scene::AddInstancedSpriteTransform: Exceeded max instanced sprite transform count for buffer 0.");
            return;
        }

        ME::Transform* transform = new ME::Transform();
        transform->SetPosition(position);
        transform->SetScale(scale);
        instancedSpriteTransforms0[instancedSpriteTransformCount0] = transform;
        ++instancedSpriteTransformCount0;
    } else {
        if (instancedSpriteTransformCount1 >= Constants::MaxInstancedSpriteTransformCount) {
            ME::LogError(
                "Scene::AddInstancedSpriteTransform: Exceeded max instanced sprite transform count for buffer 1.");
            return;
        }

        ME::Transform* transform = new ME::Transform();
        transform->SetPosition(position);
        transform->SetScale(scale);
        instancedSpriteTransforms1[instancedSpriteTransformCount1] = transform;
        ++instancedSpriteTransformCount1;
    }
}

void ME::Scene::AddInstancedSpriteRenderer(ME::SpriteRenderer* spriteRenderer, uint8_t buffer) {
    if (buffer == 0) {
        if (instancedSpriteRendererCount0 >= Constants::MaxInstancedSpriteRendererCount) {
            ME::LogError(
                "Scene::AddInstancedSpriteRenderer: Exceeded max instanced sprite renderer count for buffer 0.");
            return;
        }

        instancedSpriteRenderers0[instancedSpriteRendererCount0] = spriteRenderer;
        ++instancedSpriteRendererCount0;
    } else {
        if (instancedSpriteRendererCount1 >= Constants::MaxInstancedSpriteRendererCount) {
            ME::LogError(
                "Scene::AddInstancedSpriteRenderer: Exceeded max instanced sprite renderer count for buffer 1.");
            return;
        }

        instancedSpriteRenderers1[instancedSpriteRendererCount1] = spriteRenderer;
        ++instancedSpriteRendererCount1;
    }
}

void ME::Scene::Update() {
    UpdateSpriteRenderers();
    UpdateInstancedSpriteRenderers();
}

void ME::Scene::UpdateSpriteRenderers() {
    // Updating transforms and atlas indices for dirty sprites.
    // In separate loops to avoid cache misses.
    for (uint32_t i = 0; i < spriteRendererCount; ++i) {
        if (!spriteRenderers[i]->bDirty) {
            continue;
        }
        // Updated any required data before rendering.
    }

    for (uint32_t i = 0; i < spriteRendererCount; ++i) {
        if (spriteRenderers[i]->bDirty) {
            spriteRenderers[i]->bDirty = false;
        }
    }
}

void ME::Scene::UpdateInstancedSpriteRenderers() {
    // Updating transforms and atlas indicesfor dirty instances.
    // In separate loops to avoid cache misses.

    for (uint32_t i = 0; i < instancedSpriteRendererCount0; ++i) {
        if (!instancedSpriteRenderers0[i]->bDirty) {
            continue;
        }
        spriteInstanceData0[i].modelMatrixData = instancedSpriteTransforms0[i]->GetModelMatrix().GetDataForShader();
    }

    for (uint32_t i = 0; i < instancedSpriteRendererCount0; ++i) {
        if (!instancedSpriteRenderers0[i]->bDirty) {
            continue;
        }
        spriteInstanceData0[i].atlasIndex = instancedSpriteRenderers0[i]->atlasIndex;
        spriteInstanceData0[i].color = instancedSpriteRenderers0[i]->color;
        spriteInstanceData0[i].flags = instancedSpriteRenderers0[i]->flags;
    }

    for (uint32_t i = 0; i < instancedSpriteRendererCount0; ++i) {
        if (instancedSpriteRenderers0[i]->bDirty) {
            instancedSpriteRenderers0[i]->bDirty = false;
        }
    }

    for (uint32_t i = 0; i < instancedSpriteRendererCount1; ++i) {
        if (!instancedSpriteRenderers1[i]->bDirty) {
            continue;
        }
        spriteInstanceData1[i].modelMatrixData = instancedSpriteTransforms1[i]->GetModelMatrix().GetDataForShader();
    }

    for (uint32_t i = 0; i < instancedSpriteRendererCount1; ++i) {
        if (!instancedSpriteRenderers1[i]->bDirty) {
            continue;
        }
        spriteInstanceData1[i].atlasIndex = instancedSpriteRenderers1[i]->atlasIndex;
        spriteInstanceData1[i].color = instancedSpriteRenderers1[i]->color;
        spriteInstanceData1[i].flags = instancedSpriteRenderers1[i]->flags;
    }

    for (uint32_t i = 0; i < instancedSpriteRendererCount1; ++i) {
        if (instancedSpriteRenderers1[i]->bDirty) {
            instancedSpriteRenderers1[i]->bDirty = false;
        }
    }
}
