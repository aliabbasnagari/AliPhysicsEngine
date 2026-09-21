#pragma once

#include "math/Vec2.h"
#include "physics/IForceReceiver.h"

enum class IntegrationMode
{
    ExplicitEuler,
    SemiImplicitEuler
};

class Particle : public IForceReceiver
{
public:
    Vec2 position;
    Vec2 velocity;

    float mass;
    float inverseMass;

    Vec2 forceAccumulator;

    // inverseMass == 0 means infinite mass.
    // Infinite-mass particles are static and never move.

    Particle(
        Vec2 position = Vec2(0.0f, 0.0f),
        Vec2 velocity = Vec2(0.0f, 0.0f),
        float mass = 1.0f);

    void applyForce(Vec2 force) override;
    void clearForces();

    float getMass() const override { return mass; }
    float getInverseMass() const override { return inverseMass; }
    Vec2 getVelocity(float /*dt*/) const override { return velocity; }

    void integrate(float dt, IntegrationMode mode = IntegrationMode::ExplicitEuler);
};