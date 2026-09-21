#pragma once

#include <memory>
#include <vector>

#include "physics/ForceGenerator.h"
#include "physics/Particle.h"
#include "physics/VerletParticle.h"

class PhysicsWorld
{
public:
    PhysicsWorld();

    void step(float fixedDt);

    const Particle &getSemiImplicitParticle() const;
    const VerletParticle &getVerletParticle() const;

private:
    std::vector<std::unique_ptr<ForceGenerator>> forceGenerators;
    Particle semiImplicitParticle;
    VerletParticle verletParticle;

    Vec2 anchor;

    float restLength;
    float springConstant;
};