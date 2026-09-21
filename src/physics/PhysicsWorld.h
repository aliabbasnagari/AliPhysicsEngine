#pragma once

#include "physics/Particle.h"
#include "physics/VerletParticle.h"

class PhysicsWorld
{
public:
    PhysicsWorld();

    void step(float fixedDt);

    const Particle &getParticle() const;
    const Particle &getExplicitParticle() const;
    const Particle &getSemiImplicitParticle() const;
    const VerletParticle &getVerletParticle() const;

private:
    Particle particle;

    Particle explicitParticle;
    Particle semiImplicitParticle;

    VerletParticle verletParticle;

    Vec2 anchor;

    float restLength;
    float springConstant;
};