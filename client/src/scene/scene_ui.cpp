#include "scene_ui.h"

#include <cstddef>

#include "../utils/json_utils.h"
#include "src/misc/game_constants.h"

ME::SceneUI::SceneUI() {}

ME::SceneUI::~SceneUI() {
    delete[] spriteTexturePaths;

    for (uint32_t i = 0; i < uiSpriteRendererCount; ++i) {
        delete uiSpriteTransforms[i];
        delete uiSpriteRenderers[i];
    }
    delete[] uiSpriteTransforms;
    delete[] uiSpriteRenderers;
    delete[] uiSpriteInstanceData;

    for (uint32_t i = 0; i < textTransformsCount; ++i) {
        delete textTransforms[i];
    }
    delete[] textTransforms;

    for (uint32_t i = 0; i < textRendererCount; ++i) {
        delete textRenderers[i];
    }
    delete[] textRenderers;

    delete[] textInstanceData;
}

void ME::SceneUI::Update() {
    UpdateUISpriteRenderers();
    UpdateTextRenderers();
}

void ME::SceneUI::Init() {
    CreateResources();
    BuildUISprites();
    BuildTextRenderers();
}

void ME::SceneUI::CreateResources() {
    spriteTexturePaths = new const char*[Constants::MaxSpriteTextureCount];
    textureAtlasProperties = new ME::TextureAtlasProperties[Constants::MaxTextureAtlasPropertiesCount];

    uiSpriteTransforms = new ME::Transform*[Constants::MaxUISpriteTransformCount];
    uiSpriteRenderers = new ME::SpriteRenderer*[Constants::MaxUISpriteRendererCount];
    uiSpriteInstanceData = new ME::UISpriteRendererInstanceData[Constants::MaxUISpriteInstanceDataCount];
    textTransforms = new ME::Transform*[Constants::MaxTextTransformsCount];
    textRenderers = new ME::TextRenderer*[Constants::MaxTextRendererCount];
    textInstanceData = new ME::TextRendererInstanceData[Constants::MaxTextInstanceDataCount];

    spriteTextureCount = 0;

    textureAtlasPropertiesCount = 0;
}

void ME::SceneUI::BuildUISprites() {}

void ME::SceneUI::BuildTextRenderers() {}

void ME::SceneUI::AddUISprite(ME::Vec3 position, ME::Vec3 scale, ME::SpriteRenderer* spriteRenderer) {
    ME::Transform* transform = new ME::Transform();
    transform->SetPosition(position);
    transform->SetScale(scale);
    uiSpriteTransforms[uiSpriteRendererCount] = transform;

    uiSpriteRenderers[uiSpriteRendererCount] = spriteRenderer;
    ++uiSpriteRendererCount;
}

void ME::SceneUI::AddUIText(ME::Vec3 position, ME::Vec3 scale, ME::TextRenderer* textRenderer) {
    ME::Transform* transform = new ME::Transform();
    transform->SetPosition(position);
    transform->SetScale(scale);
    textTransforms[textRendererCount] = transform;
    textRenderers[textRendererCount] = textRenderer;
    ++textRendererCount;
    textInstanceDataCount += textRenderer->GetCount();
}

void ME::SceneUI::UpdateUISpriteRenderers() {
    // Updating transforms and atlas indicesfor dirty instances.
    // In separate loops to avoid cache misses.

    for (uint32_t i = 0; i < uiSpriteRendererCount; ++i) {
        if (!uiSpriteRenderers[i]->bDirty) {
            continue;
        }
        uiSpriteInstanceData[i].modelMatrixData = uiSpriteTransforms[i]->GetModelMatrix().GetDataForShader();
    }

    for (uint32_t i = 0; i < uiSpriteRendererCount; ++i) {
        if (!uiSpriteRenderers[i]->bDirty) {
            continue;
        }
        uiSpriteInstanceData[i].atlasIndex = uiSpriteRenderers[i]->atlasIndex;
        uiSpriteInstanceData[i].color = uiSpriteRenderers[i]->color;
        uiSpriteInstanceData[i].flags = uiSpriteRenderers[i]->flags;
    }

    for (uint32_t i = 0; i < uiSpriteRendererCount; ++i) {
        if (uiSpriteRenderers[i]->bDirty) {
            uiSpriteRenderers[i]->bDirty = false;
        }
    }
}

void ME::SceneUI::UpdateTextRenderers() {
    // Update transform data.
    uint32_t count = 0;
    for (uint32_t i = 0; i < textRendererCount; ++i) {
        if (!textRenderers[i]->bDirty) {
            count += textRenderers[i]->GetCount();
            continue;
        }

        for (int j = 0; j < textRenderers[i]->GetCount(); ++j) {
            ME::Transform tempTransform;
            ME::Vec3 pos = textTransforms[i]->GetPosition();
            pos.x += (j * (textRenderers[i]->width + textRenderers[i]->letterSpacing));
            tempTransform.SetPosition(pos);
            tempTransform.SetScale(textRenderers[i]->width, textRenderers[i]->height);
            textInstanceData[count].modelMatrixData = tempTransform.GetModelMatrix().GetDataForShader();
            ++count;
        }
    }

    // Update atlas indices and colors.
    count = 0;
    for (uint32_t i = 0; i < textRendererCount; ++i) {
        if (!textRenderers[i]->bDirty) {
            count += textRenderers[i]->GetCount();
            continue;
        }

        for (int j = 0; j < textRenderers[i]->GetCount(); ++j) {
            textInstanceData[count].atlasIndex = textRenderers[i]->text[j];
            textInstanceData[count].color = textRenderers[i]->color;
            ++count;
        }
    }

    // TODO: change only if bDirty.
    textInstanceDataCount = 0;
    for (uint32_t i = 0; i < textRendererCount; ++i) {
        textInstanceDataCount += textRenderers[i]->GetCount();
    }

    for (uint32_t i = 0; i < textRendererCount; ++i) {
        if (textRenderers[i]->bDirty) {
            textRenderers[i]->bDirty = false;
        }
    }
}
