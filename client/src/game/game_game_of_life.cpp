#include "game_game_of_life.h"

ME::GameOfLife::GameOfLife() : Game() {}

ME::GameOfLife::~GameOfLife() {}

void ME::GameOfLife::Init(ME::Time::TimeManager *currentTimeManager) {
    Game::Init(currentTimeManager);
    scene = new ME::SceneGameOfLife();
    scene->Init();
    golScene = dynamic_cast<ME::SceneGameOfLife *>(scene);
    physicsScene = new ME::PhysicsScene();
    physicsScene->Init(scene);
    physicsSystem->SetGame(this);
    physicsSystem->SetScene(physicsScene);

    currentGen = new ME::Grid<uint8_t>(gridWidth, gridHeight);
    nextGen = new ME::Grid<uint8_t>(gridWidth, gridHeight);
    tempGrid = currentGen;

    InitializeGameOfLifeLogic();

    ME::Log("Game of Life Start!");
}

void ME::GameOfLife::Update(double deltaTime) {
    Game::Update(deltaTime);

    frameCounter++;
    if (frameCounter > updateIntervalFrames) {
        frameCounter = 0;

        if (bBriansBrainMode) {
            UpdateGameOfLifeLogicBriansBrain();
        } else {
            UpdateGameOfLifeLogic();
        }
    }
}

void ME::GameOfLife::End() {
    delete scene;
    delete physicsScene;
    delete currentGen;
    delete nextGen;

    Game::End();
}

void ME::GameOfLife::InitializeGameOfLifeLogic() {
    ME::Random rnd;
    for (size_t y = 0; y < gridHeight; y++) {
        for (size_t x = 0; x < gridWidth; x++) {
            float r = rnd.NextDouble();

            if (bBriansBrainMode) {
                // Brian's Brain initialization: Randomly set cells to "on" (2) or "off" (0).
                if (r < 0.25f) {
                    *(currentGen->GetUnsafe(x, y)) = 2;
                } else {
                    *(currentGen->GetUnsafe(x, y)) = 0;
                }
                continue;
            } else {
                if (r < 0.3f) {
                    *(currentGen->GetUnsafe(x, y)) = 1;
                } else {
                    *(currentGen->GetUnsafe(x, y)) = 0;
                }
            }
        }
    }
}

void ME::GameOfLife::UpdateGameOfLifeLogic() {
    uint8_t *currentGenPtr = currentGen->GetData();
    uint8_t *nextGenPtr = nextGen->GetData();

    for (size_t y = 0; y < gridHeight; y++) {
        for (size_t x = 0; x < gridWidth; x++) {
            uint8_t *neighs[8];
            currentGen->GetNeighbors8(x, y, neighs);

            size_t liveNeighbors = 0;
            for (size_t n = 0; n < 8; n++) {
                if (neighs[n] != nullptr && *(neighs[n]) == 1) {
                    liveNeighbors++;
                }
            }

            uint8_t currentCell = *(currentGen->GetUnsafe(x, y));
            if (currentCell == 1) {
                // Any live cell with two or three live neighbours survives.
                if (liveNeighbors == 2 || liveNeighbors == 3) {
                    *(nextGen->GetUnsafe(x, y)) = 1;
                } else {
                    // All other live cells die in the next generation.
                    *(nextGen->GetUnsafe(x, y)) = 0;
                }
            } else {
                // Any dead cell with exactly three live neighbours becomes a live cell.
                if (liveNeighbors == 3) {
                    *(nextGen->GetUnsafe(x, y)) = 1;
                } else {
                    // All other dead cells stay dead.
                    *(nextGen->GetUnsafe(x, y)) = 0;
                }
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
            golScene->spriteInstanceData0[i].color = ME::Color("#a7ce47");
        } else {
            golScene->spriteInstanceData0[i].color = ME::Color::Black();
        }
    }
}

void ME::GameOfLife::UpdateGameOfLifeLogicBriansBrain() {
    uint8_t *currentGenPtr = currentGen->GetData();
    uint8_t *nextGenPtr = nextGen->GetData();

    for (size_t y = 0; y < gridHeight; y++) {
        for (size_t x = 0; x < gridWidth; x++) {
            uint8_t *neighs[8];
            currentGen->GetNeighbors8(x, y, neighs);

            size_t liveNeighbors = 0;
            for (size_t n = 0; n < 8; n++) {
                if (neighs[n] != nullptr && *(neighs[n]) == 2) {
                    liveNeighbors++;
                }
            }

            uint8_t currentCell = *(currentGen->GetUnsafe(x, y));
            // All living cell go to dying.
            if (currentCell == 2) {
                *(nextGen->GetUnsafe(x, y)) = 1;
            }
            // All dying cells go to dead.
            else if (currentCell == 1) {
                *(nextGen->GetUnsafe(x, y)) = 0;
            } else {
                // Any dead cell with exactly two live neighbours becomes a living cell.
                if (liveNeighbors == birthNumber) {
                    *(nextGen->GetUnsafe(x, y)) = 2;
                } else {
                    // All other dead cells stay dead.
                    *(nextGen->GetUnsafe(x, y)) = 0;
                }
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
        if (*(cellPtr + i) == 2) {
            golScene->spriteInstanceData0[i].color = ME::Color("#deeff3");
        } else if (*(cellPtr + i) == 1) {
            golScene->spriteInstanceData0[i].color = ME::Color("#f0532c");
        } else {
            golScene->spriteInstanceData0[i].color = ME::Color::Black();
        }
    }
}
