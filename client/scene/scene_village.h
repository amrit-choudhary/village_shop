/**
 * The village world: tiles, villagers and shops will be built here through the Scene Build* overrides.
 * Empty for now, so it renders as a plain background.
 */
#pragma once

#include "moksha/client/scene/scene.h"

namespace VS {

class SceneVillage : public ME::Scene {
   public:
    SceneVillage() = default;
    ~SceneVillage() override = default;

    const char* GetDisplayName() const override;
};

}  // namespace VS
