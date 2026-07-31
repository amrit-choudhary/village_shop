#include "scene_ui.h"

#include <cstddef>

#include "src/misc/game_constants.h"
#include "src/utils/json_utils.h"

ME::SceneUI::SceneUI() {}

ME::SceneUI::~SceneUI() {
    delete[] spriteTexturePaths;

    // Note: individual entries are no longer deleted here. Ownership of sprite/text
    // renderers+transforms (and of UIElements themselves) belongs to whoever created them
    // (e.g. Panel/Label/Image own their own renderer+Transform) — SceneUI only ever holds
    // non-owning pointers into these arrays now, via RebuildUISprites/RebuildUIText/
    // AddUIElement. Only the array containers themselves are freed.
    delete[] uiSpriteTransforms;
    delete[] uiSpriteRenderers;
    delete[] uiSpriteInstanceData;

    delete[] textTransforms;
    delete[] textRenderers;
    delete[] textInstanceData;

    delete[] uiElements;
}

void ME::SceneUI::Update() {
    UpdateUISpriteRenderers();
    UpdateTextRenderers();
}

void ME::SceneUI::Init() {
    CreateResources();
    BuildUISprites();
    BuildTextRenderers();
    BuildUIElements();
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

    uiElements = new ME::UIElement*[Constants::MaxUIElementCount];

    spriteTextureCount = 0;

    textureAtlasPropertiesCount = 0;
}

void ME::SceneUI::BuildUISprites() {}

void ME::SceneUI::BuildTextRenderers() {}

void ME::SceneUI::BuildUIElements() {}

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

bool ME::SceneUI::AddUIElement(ME::UIElement* element) {
    if (uiElementCount >= Constants::MaxUIElementCount) {
        return false;
    }
    uiElements[uiElementCount] = element;
    ++uiElementCount;
    return true;
}

void ME::SceneUI::RemoveUIElement(ME::UIElement* element) {
    for (uint32_t i = 0; i < uiElementCount; ++i) {
        if (uiElements[i] == element) {
            uiElements[i] = uiElements[uiElementCount - 1];
            --uiElementCount;
            return;
        }
    }
}

ME::UIElement** ME::SceneUI::GetUIElements() const {
    return uiElements;
}

uint32_t ME::SceneUI::GetUIElementCount() const {
    return uiElementCount;
}

void ME::SceneUI::RebuildUISprites(ME::Transform** transforms, ME::SpriteRenderer** renderers, uint32_t count) {
    uint32_t boundedCount = count < Constants::MaxUISpriteRendererCount ? count : Constants::MaxUISpriteRendererCount;
    for (uint32_t i = 0; i < boundedCount; ++i) {
        uiSpriteTransforms[i] = transforms[i];
        uiSpriteRenderers[i] = renderers[i];
    }
    uiSpriteRendererCount = boundedCount;
    // One instance per sprite renderer (unlike text, which can have multiple glyph instances
    // per TextRenderer) — UpdateUISpriteRenderers() doesn't recompute this itself, unlike
    // UpdateTextRenderers()'s textInstanceDataCount, so it must be set here.
    uiSpriteInstanceDataCount = boundedCount;
}

void ME::SceneUI::RebuildUIText(ME::Transform** transforms, ME::TextRenderer** renderers, uint32_t count) {
    uint32_t boundedCount = count < Constants::MaxTextRendererCount ? count : Constants::MaxTextRendererCount;
    for (uint32_t i = 0; i < boundedCount; ++i) {
        textTransforms[i] = transforms[i];
        textRenderers[i] = renderers[i];
    }
    textRendererCount = boundedCount;
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

        float totalWidth = static_cast<float>(textRenderers[i]->GetRenderWidth());
        float glyphStep = static_cast<float>(textRenderers[i]->width + textRenderers[i]->letterSpacing);
        for (int j = 0; j < textRenderers[i]->GetCount(); ++j) {
            ME::Transform tempTransform;
            ME::Vec3 pos = textTransforms[i]->GetPosition();
            pos.x += -totalWidth / 2.0f + j * glyphStep + textRenderers[i]->width / 2.0f;
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
