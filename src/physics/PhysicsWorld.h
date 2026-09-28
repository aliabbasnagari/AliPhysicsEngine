#pragma once

#include <memory>
#include <vector>

#include "physics/ForceGenerator.h"
#include "physics/SpringForceGenerator.h"
#include "physics/DistanceConstraint.h"
#include "physics/Particle.h"
#include "physics/VerletParticle.h"

class PhysicsWorld
{
public:
    Particle *createParticle(
        const Vec2 &position,
        const Vec2 &velocity,
        float mass,
        IntegrationMode mode = IntegrationMode::SemiImplicitEuler);

    VerletParticle *createVerletParticle(
        const Vec2 &position,
        const Vec2 &velocity,
        float mass,
        float dt);

    // Returns a non-owning pointer to the generator just registered,
    // so a caller (e.g. a debug UI) can keep tuning it afterward
    // without PhysicsWorld exposing its internal list.
    ForceGenerator *addForceGenerator(
        std::unique_ptr<ForceGenerator> generator);

    // Removes every registered generator without touching particles.
    // Deliberately generic — no gravity/drag-specific logic here —
    // so any future generator (springs, wind, constraints) uses this
    // exact call to be added or removed.
    void clearForceGenerators();

    SpringForceGenerator *addSpring(std::unique_ptr<SpringForceGenerator> spring);
    void clearSprings();

    DistanceConstraint *addConstraint(
        std::unique_ptr<DistanceConstraint> constraint);
    void clearConstraints();

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
    std::vector<std::unique_ptr<SpringForceGenerator>> springs;
    std::vector<std::unique_ptr<DistanceConstraint>> constraints;
};