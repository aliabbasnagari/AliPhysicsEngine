#pragma once

#include "math/Vec2.h"
#include "physics/IForceReceiver.h"

// Verlet is convenient for constraint solving because constraints
// can directly modify position; the next step automatically derives
// the resulting velocity from the updated position history.
class VerletParticle : public IForceReceiver
{
public:
    Vec2 oldPosition;
    Vec2 position;

    float mass;
    float inverseMass;

    Vec2 forceAccumulator;

    // inverseMass == 0 means infinite mass.
    // Infinite-mass particles are static and never move.

    VerletParticle(
        Vec2 position = Vec2(0.0f, 0.0f),
        Vec2 velocity = Vec2(0.0f, 0.0f),
        float mass = 1.0f,
        float fixedDt = 1.0f / 60.0f);

    void applyForce(Vec2 force) override;
    void clearForces();

    void integrate(float dt);

    float getMass() const override { return mass; }
    float getInverseMass() const override { return inverseMass; }
    Vec2 getVelocity(float dt) const override;
};