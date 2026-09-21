#pragma once

#include "math/Vec2.h"

class VerletParticle
{
public:
    Vec2 oldPosition;
    Vec2 position;

    float mass;
    float inverseMass;

    // inverseMass == 0 means infinite mass.
    // Infinite-mass particles are static and never move.

    VerletParticle(
        Vec2 position = Vec2(0.0f, 0.0f),
        Vec2 velocity = Vec2(0.0f, 0.0f),
        float mass = 1.0f,
        float fixedDt = 1.0f / 60.0f);

    void integrate(float dt, Vec2 acceleration);

    Vec2 getVelocity(float dt) const;
};