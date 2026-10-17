/**
 * The Village Shop game: owns the village scene and its UI. Currently an empty village with a title bar;
 * the shop and village simulation (design/v1/design_v1.md) are built on top of it.
 */
#pragma once

#include "moksha/client/game/game.h"

namespace VS {

class SceneUIVillage;
class SceneVillage;

class VillageGame : public ME::Game {
   public:
    VillageGame() = default;
    ~VillageGame() override = default;

    VillageGame(const VillageGame&) = delete;
    VillageGame& operator=(const VillageGame&) = delete;
    VillageGame(VillageGame&&) = delete;
    VillageGame& operator=(VillageGame&&) = delete;

    // Creates the village scene and its UI.
    void Init(ME::Engine& inEngine) override;

    // Deletes both scenes.
    void End() override;

    const char* GetDisplayName() const override;

   private:
    // The same objects as the base scene/uiScene, kept with their concrete types.
    SceneVillage* villageScene = nullptr;
    SceneUIVillage* villageUIScene = nullptr;
};

}  // namespace VS
