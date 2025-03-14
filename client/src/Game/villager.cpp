#include <random>

#include "villager.h"

// Health related functions.
VHealth::VHealth()
{
    Health = 100.0f;
}

VHealth::~VHealth()
{
}

void VHealth::Update(double deltaTime)
{
}

// Hunger related functions.

VHunger::VHunger()
{
    Hunger = 0;
    hungerRate = (float)std::rand() / RAND_MAX;
}

VHunger::VHunger(float inHungerRate)
{
    Hunger = 0;
    hungerRate = inHungerRate;
}

VHunger::~VHunger()
{
}

void VHunger::Update(double deltaTime)
{
    Hunger += hungerRate * deltaTime;
    if (Hunger > 100)
    {
        Hunger = 100;
    }
}

// Gold related function.

VGold::VGold()
{
    Gold = 10;
}

VGold::~VGold()
{
}

void VGold::Update(double deltaTime)
{
}