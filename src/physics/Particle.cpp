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

void Particle::integrate(float dt)
{
    // inverseMass == 0 means infinite mass / static particle.
    if (inverseMass == 0.0f)
        return;

    constexpr Vec2 gravity(0.0f, -9.81f);

    // Explicit Euler:
    // Use velocity and acceleration from the START
    // of this timestep.

    position += velocity * dt;
    velocity += gravity * dt;
}