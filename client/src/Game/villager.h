/**
 * All the important Villager parameters.
 * Save in independent arrays to be cache friendly.
 */
#pragma once

class VHealth
{
public:
    VHealth();
    ~VHealth();
    float Health;
    void Update(double deltaTime);
};

class VHunger
{
public:
    VHunger();
    VHunger(float inHungerRate);
    ~VHunger();
    float Hunger;
    void Update(double deltaTime);

private:
    float hungerRate;
};

class VGold
{
public:
    VGold();
    ~VGold();
    float Gold;
    void Update(double deltaTime);
};