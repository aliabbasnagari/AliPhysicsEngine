#include "physics/PhysicsWorld.h"

Particle *PhysicsWorld::createParticle(const Vec2 &position, const Vec2 &velocity, float mass, IntegrationMode mode)
{
    particles.push_back(
        std::make_unique<Particle>(position, velocity, mass));
    particles.back()->setIntegrationMode(mode);
    return particles.back().get();
}

VerletParticle *PhysicsWorld::createVerletParticle(
    const Vec2 &position, const Vec2 &velocity, float mass, float dt)
{
    verletParticles.push_back(
        std::make_unique<VerletParticle>(position, velocity, mass, dt));
    return verletParticles.back().get();
}

ForceGenerator *PhysicsWorld::addForceGenerator(
    std::unique_ptr<ForceGenerator> generator)
{
    forceGenerators.push_back(std::move(generator));
    return forceGenerators.back().get();
}

void PhysicsWorld::clearForceGenerators()
{
    forceGenerators.clear();
}

SpringForceGenerator *PhysicsWorld::addSpring(
    std::unique_ptr<SpringForceGenerator> spring)
{
    springs.push_back(std::move(spring));
    return springs.back().get();
}

void PhysicsWorld::clearSprings()
{
    springs.clear();
}

void PhysicsWorld::step(float fixedDt)
{
    // 1. Clear.
    for (auto &particle : particles)
    {
        particle->clearForces();
    }
    for (auto &particle : verletParticles)
    {
        particle->clearForces();
    }

    // 2a. Single-receiver generators.
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

    // 2b. Pairwise springs — each call touches both its endpoints.
    for (const auto &spring : springs)
    {
        spring->updateForce(fixedDt);
    }

    // 3. Integrate.
    for (auto &particle : particles)
    {
        particle->integrate(fixedDt, particle->getIntegrationMode());
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
    springs.clear();
}