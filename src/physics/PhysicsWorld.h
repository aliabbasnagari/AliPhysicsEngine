#pragma once

#include "physics/Particle.h"

class PhysicsWorld
{
public:
    PhysicsWorld();

    void step(float fixedDt);

    const Particle &getParticle() const;
    const Particle &getExplicitParticle() const;
    const Particle &getSemiImplicitParticle() const;

private:
    Particle particle;

    Particle explicitParticle;
    Particle semiImplicitParticle;

    Vec2 anchor;

    float restLength;
    float springConstant;
};