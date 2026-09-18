#include "physics/Particle.h"

Particle::Particle(
    Vec2 position,
    Vec2 velocity,
    float mass)
    : position(position),
      velocity(velocity),
      mass(mass),
      inverseMass(mass > 0.0f ? 1.0f / mass : 0.0f)
{
}

void Particle::integrate(float dt, Vec2 acceleration, IntegrationMode mode)
{
    // inverseMass == 0 means infinite mass / static particle.
    if (inverseMass == 0.0f)
        return;

    // Explicit Euler:
    // Use velocity and acceleration from the START
    // of this timestep.

    if (mode == IntegrationMode::ExplicitEuler)
    {
        position += velocity * dt;
        velocity += acceleration * dt;
    }
    else
    {
        velocity += acceleration * dt;
        position += velocity * dt;
    }
}