#include "creature.h"

ME::Creature::Creature() {}

ME::Creature::~Creature() {}

void ME::Creature::Update(float deltaTime, const Grid<uint8_t>* walkableMap, Random& rnd) {
    moveCooldown -= deltaTime;
    if (moveCooldown > 0.0f) {
        return;
    }

    Move(walkableMap, rnd);
    moveCooldown = kMoveCooldownDuration;
}

void ME::Creature::Move(const Grid<uint8_t>* walkableMap, Random& rnd) {
    // Matches Grid::GetNeighbors8 slot order: E, NE, N, NW, W, SW, S, SE.
    static const int kOffsetsX[8] = {1, 1, 0, -1, -1, -1, 0, 1};
    static const int kOffsetsY[8] = {0, 1, 1, 1, 0, -1, -1, -1};

    size_t x = static_cast<size_t>(position.x);
    size_t y = static_cast<size_t>(position.y);

    const uint8_t* neighbors[8];
    walkableMap->GetNeighbors8(x, y, neighbors);

    size_t validSlots[8];
    size_t validCount = 0;
    for (size_t i = 0; i < 8; ++i) {
        if (neighbors[i] != nullptr && *neighbors[i] == 1) {
            validSlots[validCount] = i;
            ++validCount;
        }
    }

    if (validCount == 0) {
        return;
    }

    size_t slot = validSlots[rnd.NextRange(0, static_cast<uint32_t>(validCount - 1))];
    position.x += static_cast<float>(kOffsetsX[slot]);
    position.y += static_cast<float>(kOffsetsY[slot]);
}
