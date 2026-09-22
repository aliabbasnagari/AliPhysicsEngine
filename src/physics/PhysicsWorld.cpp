#include "physics/PhysicsWorld.h"

Particle *PhysicsWorld::createParticle(
    const Vec2 &position,
    const Vec2 &velocity,
    float mass)
{
    particles.push_back(
        std::make_unique<Particle>(position, velocity, mass));

    return particles.back().get();
}

VerletParticle *PhysicsWorld::createVerletParticle(
    const Vec2 &position,
    const Vec2 &velocity,
    float mass,
    float dt)
{
    verletParticles.push_back(
        std::make_unique<VerletParticle>(
            position, velocity, mass, dt));

    return verletParticles.back().get();
}

void PhysicsWorld::addForceGenerator(
    std::unique_ptr<ForceGenerator> generator)
{
    forceGenerators.push_back(std::move(generator));
}

void PhysicsWorld::step(float fixedDt)
{
    for (auto &particle : particles)
    {
        particle->clearForces();
    }

    for (auto &particle : verletParticles)
    {
        particle->clearForces();
    }

    // Every generator currently affects every particle.
    for (const auto &generator : forceGenerators)
    {
        for (auto &particle : particles)
        {
            generator->updateForce(*particle, fixedDt);
        }

        for (auto &particle : verletParticles)
        {
            generator->updateForce(*particle, fixedDt);
        }
    }

    for (auto &particle : particles)
    {
        particle->integrate(
            fixedDt,
            IntegrationMode::SemiImplicitEuler);
    }

    for (auto &particle : verletParticles)
    {
        particle->integrate(fixedDt);
    }
}

void PhysicsWorld::clear()
{
    particles.clear();
    verletParticles.clear();
    forceGenerators.clear();
}