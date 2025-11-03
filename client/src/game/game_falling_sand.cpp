#include "game_falling_sand.h"

ME::GameFallingSand::GameFallingSand() : Game() {}

ME::GameFallingSand::~GameFallingSand() {}

void ME::GameFallingSand::Init(ME::Time::TimeManager *currentTimeManager) {
    Game::Init(currentTimeManager);
    scene = new ME::SceneFallingSand();
    scene->Init();
    sandScene = dynamic_cast<ME::SceneFallingSand *>(scene);
    physicsScene = new ME::PhysicsScene();
    physicsScene->Init(scene);
    physicsSystem->SetGame(this);
    physicsSystem->SetScene(physicsScene);

    currentGen = new ME::Grid<uint8_t>(gridWidth, gridHeight);
    nextGen = new ME::Grid<uint8_t>(gridWidth, gridHeight);
    tempGrid = currentGen;

    InitializeFallingSandLogic();

    ME::Log("Falling Sand Start!");
}

void ME::GameFallingSand::Update(double deltaTime) {
    Game::Update(deltaTime);

    frameCounter++;
    if (frameCounter > updateIntervalFrames) {
        frameCounter = 0;

        UpdateFallingSandLogic();
    }
}

void ME::GameFallingSand::End() {
    delete scene;
    delete physicsScene;
    delete currentGen;
    delete nextGen;

    Game::End();
}

void ME::GameFallingSand::InitializeFallingSandLogic() {
    ME::Random rnd;
    for (size_t y = 0; y < gridHeight; ++y) {
        for (size_t x = 0; x < gridWidth; ++x) {
            float r = rnd.NextDouble();

            if (r < 0.01f) {
                *(currentGen->GetUnsafe(x, y)) = 1;
            } else {
                *(currentGen->GetUnsafe(x, y)) = 0;
            }
        }
    }
}

void ME::GameFallingSand::UpdateFallingSandLogic() {
    ME::Random rnd;
    size_t y = gridHeight - 1;
    for (size_t x = 10; x < 20; ++x) {
        float r = rnd.NextDouble();
        if (r < 0.07f) {
            *(currentGen->GetUnsafe(x, y)) = 1;
        } else {
            *(currentGen->GetUnsafe(x, y)) = 0;
        }
    }

    for (size_t x = 40; x < gridWidth - 40; ++x) {
        float r = rnd.NextDouble();
        if (r < 0.10f) {
            *(currentGen->GetUnsafe(x, y)) = 1;
        } else {
            *(currentGen->GetUnsafe(x, y)) = 0;
        }
    }

    for (size_t x = gridWidth - 20; x < gridWidth - 10; ++x) {
        float r = rnd.NextDouble();
        if (r < 0.07f) {
            *(currentGen->GetUnsafe(x, y)) = 1;
        } else {
            *(currentGen->GetUnsafe(x, y)) = 0;
        }
    }

    uint8_t *currentGenPtr = currentGen->GetData();
    uint8_t *nextGenPtr = nextGen->GetData();

    for (size_t y = 0; y < gridHeight; ++y) {
        for (size_t x = 0; x < gridWidth; ++x) {
            const uint8_t me = *(currentGen->GetUnsafe(x, y));
            const uint8_t *down = currentGen->GetNeighbor(x, y, ME::GridDirection::S);
            const uint8_t *downLeft = currentGen->GetNeighbor(x, y, ME::GridDirection::SW);
            const uint8_t *downRight = currentGen->GetNeighbor(x, y, ME::GridDirection::SE);
            if (me == 1 && down != nullptr) {
                if (*down == 0) {
                    *(nextGen->GetUnsafe(x, y)) = 0;
                    *(nextGen->GetUnsafe(x, y - 1)) = 1;
                } else {
                    if (downLeft != nullptr && *downLeft == 0) {
                        *(nextGen->GetUnsafe(x, y)) = 0;
                        *(nextGen->GetUnsafe(x - 1, y - 1)) = 1;
                    } else if (downRight != nullptr && *downRight == 0) {
                        *(nextGen->GetUnsafe(x, y)) = 0;
                        *(nextGen->GetUnsafe(x + 1, y - 1)) = 1;
                    } else {
                        // Stay in place
                        *(nextGen->GetUnsafe(x, y)) = *(currentGen->GetUnsafe(x, y));
                    }
                }
            } else {
                // Stay in place
                *(nextGen->GetUnsafe(x, y)) = *(currentGen->GetUnsafe(x, y));
            }
        }
    }

    // Swap grids.
    tempGrid = currentGen;
    currentGen = nextGen;
    nextGen = tempGrid;

    // Update drawing.
    uint8_t *cellPtr = currentGen->GetData();
    for (size_t i = 0; i < gridCount; ++i) {
        if (*(cellPtr + i) == 1) {
            sandScene->spriteInstanceData[i]->color = ME::Color("#CBBD93");
        } else {
            sandScene->spriteInstanceData[i]->color = ME::Color::Black();
        }
    }
}
