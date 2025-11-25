#pragma once

/**
 * This defines the different physics layers used in the physics simulation.
 * Each layer can be used to categorize objects for collision detection and response.
 * Layers can be combined using bitwise operations to create complex collision rules.
 */

#include <cstdint>

namespace ME {

/**
 * Define the physics layers using bit flags.
 * Use this to define category and mask.
 */
enum class PhysicsLayer : uint32_t {
    None = 0u,
    Default = 1u << 0,      // Default layer for general objects.
    Player = 1u << 1,       // Layer for player characters.
    Enemy = 1u << 2,        // Layer for enemy characters.
    Projectile = 1u << 3,   // Layer for projectiles like bullets.
    Environment = 1u << 4,  // Layer for static environment objects.
    Trigger = 1u << 5,      // Layer for trigger volumes.
    UI = 1u << 6,           // Layer for UI elements.
    Layer0 = 1u << 7,       // Custom layer 0.
    Layer1 = 1u << 8,       // Custom layer 1.
    Layer2 = 1u << 9,       // Custom layer 2.
    Layer3 = 1u << 10,      // Custom layer 3.
    Layer4 = 1u << 11,      // Custom layer 4.
    All = 0xFFFFFFFFu       // All layers.
};

namespace Physics {

constexpr inline PhysicsLayer operator|(PhysicsLayer a, PhysicsLayer b) {
    return static_cast<PhysicsLayer>(static_cast<uint32_t>(a) | static_cast<uint32_t>(b));
}

constexpr inline PhysicsLayer operator&(PhysicsLayer a, PhysicsLayer b) {
    return static_cast<PhysicsLayer>(static_cast<uint32_t>(a) & static_cast<uint32_t>(b));
}

constexpr inline PhysicsLayer& operator|=(PhysicsLayer& a, PhysicsLayer b) {
    a = a | b;
    return a;
}

constexpr inline PhysicsLayer& operator&=(PhysicsLayer& a, PhysicsLayer b) {
    a = a & b;
    return a;
}

constexpr inline PhysicsLayer operator~(PhysicsLayer a) {
    return static_cast<PhysicsLayer>(~static_cast<uint32_t>(a));
}

/** Returns the bit representation of the given PhysicsLayer. */
constexpr inline uint32_t ToBits(PhysicsLayer layer) {
    return static_cast<uint32_t>(layer);
}

/** Checks if the given category and mask have any overlapping bits set. */
constexpr inline bool HasLayer(PhysicsLayer mask, PhysicsLayer category) {
    return (ToBits(mask) & ToBits(category)) != 0;
}

/** Collision filter test. Collision can happen if this returns true. */
constexpr inline bool IsCollisionAllowed(PhysicsLayer categoryA, PhysicsLayer maskA, PhysicsLayer categoryB,
                                         PhysicsLayer maskB) {
    return HasLayer(maskA, categoryB) && HasLayer(maskB, categoryA);
}

}  // namespace Physics
}  // namespace ME
