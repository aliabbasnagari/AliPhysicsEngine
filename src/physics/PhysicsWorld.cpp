#include "physics/PhysicsWorld.h"

PhysicsWorld::PhysicsWorld()
    : particle(Vec2(0.0f, 5.0f), Vec2(0.0f, 0.0f), 1.0f)
{
}

void PhysicsWorld::step(float fixedDt)
{
    particle.integrate(fixedDt);
}

const Particle &PhysicsWorld::getParticle() const
{
    return particle;
}