#include "physics/PhysicsWorld.h"

#include <algorithm>

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

RigidBody *PhysicsWorld::addRigidBody(RigidBody body)
{
    rigidBodies.push_back(std::make_unique<RigidBody>(std::move(body)));
    return rigidBodies.back().get();
}

// Invalidates `body`; callers must drop their own copy of the pointer.
// An unknown pointer is ignored. Do not call during step().
void PhysicsWorld::removeRigidBody(RigidBody *body)
{
    rigidBodies.erase(
        std::remove_if(
            rigidBodies.begin(), rigidBodies.end(),
            [body](const std::unique_ptr<RigidBody> &owned)
            { return owned.get() == body; }),
        rigidBodies.end());
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

DistanceConstraint *PhysicsWorld::addConstraint(
    std::unique_ptr<DistanceConstraint> constraint)
{
    constraints.push_back(std::move(constraint));
    return constraints.back().get();
}

void PhysicsWorld::clearConstraints()
{
    constraints.clear();
}

VerletDistanceConstraint *PhysicsWorld::addVerletConstraint(
    std::unique_ptr<VerletDistanceConstraint> constraint)
{
    verletConstraints.push_back(std::move(constraint));
    return verletConstraints.back().get();
}

void PhysicsWorld::clearVerletConstraints()
{
    verletConstraints.clear();
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
    for (auto &body : rigidBodies)
    {
        body->clearForces();
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
        for (auto &body : rigidBodies)
        {
            generator->updateForce(*body, fixedDt);
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
    for (auto &body : rigidBodies)
    {
        body->integrate(fixedDt);
    }

    for (int iteration = 0; iteration < constraintIterations; ++iteration)
    {
        for (const auto &constraint : constraints)
        {
            constraint->solve();
        }
        for (const auto &constraint : verletConstraints)
        {
            constraint->solve();
        }
    }
}

void PhysicsWorld::clear()
{
    particles.clear();
    verletParticles.clear();
    rigidBodies.clear();
    forceGenerators.clear();
    springs.clear();
    constraints.clear();
    verletConstraints.clear();
}