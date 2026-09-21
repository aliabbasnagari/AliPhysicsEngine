#include "physics/Particle.h"

Particle::Particle(
    Vec2 position,
    Vec2 velocity,
    float mass)
    : position(position),
      velocity(velocity),
      mass(mass),
      inverseMass(mass > 0.0f ? 1.0f / mass : 0.0f),
      forceAccumulator(0.0f, 0.0f)
{
}

void Particle::applyForce(Vec2 force)
{
    forceAccumulator += force;
}

void Particle::clearForces()
{
    forceAccumulator = Vec2(0.0f, 0.0f);
}

void Particle::integrate(
    float dt,
    IntegrationMode mode)
{
    // inverseMass == 0 means infinite mass / static particle.
    if (inverseMass == 0.0f)
        return;

    // F = ma
    // a = F / m
    // a = F * inverseMass
    Vec2 acceleration =
        forceAccumulator * inverseMass;

    if (mode == IntegrationMode::ExplicitEuler)
    {
        // Explicit Euler:
        // Position uses the OLD velocity, so this step's acceleration
        // hasn't been "felt" yet when position advances - on an
        // oscillating system that means every step overshoots a little
        // further in the same direction, so the error compounds and
        // the system's energy grows without bound over time.
        position += velocity * dt;
        velocity += acceleration * dt;
    }
    else
    {
        // Semi-implicit (symplectic) Euler:
        // Velocity is updated FIRST, so position advances using a
        // velocity that already reflects this step's acceleration.
        // The overshoot now partially cancels itself out step to step
        // instead of compounding, so energy stays roughly bounded
        // (oscillating around the true value) instead of growing -
        // that's why this ordering is stable where explicit Euler
        // blows up.
        velocity += acceleration * dt;
        position += velocity * dt;
    }

    // Forces are only valid for this timestep.
    clearForces();
}