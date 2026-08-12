#include "client/src/game/evolution/game_evolution.h"

ME::GameEvolution::GameEvolution() : Game() {}

ME::GameEvolution::~GameEvolution() {}

void ME::GameEvolution::Init(ME::Time::TimeManager* currentTimeManager) {
    Game::Init(currentTimeManager);

    evoScene = new ME::SceneEvolution();
    scene = evoScene;
    scene->Init();

    evoUIScene = new ME::SceneUIEvolution();
    uiScene = evoUIScene;
    uiScene->Init();

    evoPhysicsScene = new ME::PhysicsSceneEvolution();
    evoPhysicsScene->Init(scene->staticColliders, scene->staticColliderCount, scene->dynamicColliders,
                          scene->dynamicColliderCount);
    physicsSystem->SetCollisionListener(this);
    physicsSystem->SetScene(evoPhysicsScene);

    ME::Log("Evolution Game Start!");
}

void ME::GameEvolution::Start() {
    Game::Start();
}

void ME::GameEvolution::Update(double deltaTime) {
    Game::Update(deltaTime);

    evoScene->UpdateCreatures(static_cast<float>(deltaTime));

    const float speed = cameraSpeed * static_cast<float>(deltaTime);
    ME::Vec3 movementVector = ME::Vec3{0.0f, 0.0f, 0.0f};

    if (inputManager->GetKeyDown(ME::Input::KeyCode::W)) {
        movementVector.y += speed;
    }
    if (inputManager->GetKeyDown(ME::Input::KeyCode::S)) {
        movementVector.y -= speed;
    }
    if (inputManager->GetKeyDown(ME::Input::KeyCode::A)) {
        movementVector.x -= speed;
    }
    if (inputManager->GetKeyDown(ME::Input::KeyCode::D)) {
        movementVector.x += speed;
    }

    evoScene->spriteCamera->position += movementVector;
    evoScene->spriteCamera->viewPosition += movementVector;
}

void ME::GameEvolution::End() {
    delete evoPhysicsScene;
    evoPhysicsScene = nullptr;
    delete evoScene;
    evoScene = nullptr;
    delete evoUIScene;
    evoUIScene = nullptr;

    Game::End();
}

const char* ME::GameEvolution::GetDisplayName() const {
    return "Evolution Game";
}
