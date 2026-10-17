#include "village_game.h"

#include "client/scene/scene_ui_village.h"
#include "client/scene/scene_village.h"
#include "moksha/logging/logging.h"

void VS::VillageGame::Init(ME::Engine& inEngine) {
    Game::Init(inEngine);

    villageScene = new SceneVillage();
    villageScene->Init();
    scene = villageScene;

    villageUIScene = new SceneUIVillage();
    villageUIScene->Init();
    uiScene = villageUIScene;

    ME::Log("Village Shop started");
}

void VS::VillageGame::End() {
    delete villageUIScene;
    villageUIScene = nullptr;
    uiScene = nullptr;

    delete villageScene;
    villageScene = nullptr;
    scene = nullptr;

    Game::End();
}

const char* VS::VillageGame::GetDisplayName() const {
    return "The Village Shop";
}
