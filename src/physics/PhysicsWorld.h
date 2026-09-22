#pragma once

#include <memory>
#include <vector>

#include "physics/ForceGenerator.h"
#include "physics/Particle.h"
#include "physics/VerletParticle.h"

class PhysicsWorld
{
public:
    // Returned pointers stay valid until clear(). The world owns
    // the particles; callers must not delete them.
    Particle *createParticle(
        const Vec2 &position,
        const Vec2 &velocity,
        float mass);

    VerletParticle *createVerletParticle(
        const Vec2 &position,
        const Vec2 &velocity,
        float mass,
        float dt);

    void addForceGenerator(std::unique_ptr<ForceGenerator> generator);

    void step(float fixedDt);
    void clear();

    const std::vector<std::unique_ptr<Particle>> &getParticles() const
    {
        return particles;
    }

    const std::vector<std::unique_ptr<VerletParticle>> &
    getVerletParticles() const
    {
        return verletParticles;
    }

private:
    std::vector<std::unique_ptr<Particle>> particles;
    std::vector<std::unique_ptr<VerletParticle>> verletParticles;
    std::vector<std::unique_ptr<ForceGenerator>> forceGenerators;
};