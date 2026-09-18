#pragma once

#include "physics/Particle.h"

class PhysicsWorld
{
public:
    PhysicsWorld();

    void step(float fixedDt);

    const Particle &getParticle() const;

private:
    Particle particle;
};