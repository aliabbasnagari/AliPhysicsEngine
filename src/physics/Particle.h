#pragma once

#include "math/Vec2.h"

class Particle
{
public:
    Vec2 position;
    Vec2 velocity;

    float mass;
    float inverseMass;

    // inverseMass == 0 means infinite mass.
    // Infinite-mass particles are static and never move.

    Particle(
        Vec2 position = Vec2(0.0f, 0.0f),
        Vec2 velocity = Vec2(0.0f, 0.0f),
        float mass = 1.0f);

    void integrate(float dt);
};