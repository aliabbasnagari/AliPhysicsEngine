#include "physics/VerletParticle.h"

VerletParticle::VerletParticle(
    Vec2 position,
    Vec2 velocity,
    float mass,
    float fixedDt)
    : oldPosition(position - velocity * fixedDt),
      position(position),
      mass(mass),
      inverseMass(mass > 0.0f ? 1.0f / mass : 0.0f),
      forceAccumulator(0.0f, 0.0f)
{
}

void VerletParticle::applyForce(Vec2 force)
{
    forceAccumulator += force;
}

void VerletParticle::clearForces()
{
    forceAccumulator = Vec2(0.0f, 0.0f);
}

void VerletParticle::integrate(float dt)
{
    // Infinite-mass particles are static.
    if (inverseMass == 0.0f)
    {
        clearForces();
        return;
    }

    if (dt <= 0.0f)
    {
        clearForces();
        return;
    }

    Vec2 acceleration =
        forceAccumulator * inverseMass;

    // Save the current position before advancing.
    Vec2 currentPosition = position;

    // Verlet integration:
    //
    // x(t + dt) = 2x(t) - x(t - dt) + a * dt^2
    position =
        2.0f * position - oldPosition + acceleration * (dt * dt);

    // Move the position history forward.
    oldPosition = currentPosition;
}

Vec2 VerletParticle::getVelocity(float dt) const
{
    if (dt <= 0.0f)
        return Vec2(0.0f, 0.0f);

    // Velocity is reconstructed from position history.
    //
    // v ~= (x(t) - x(t - dt)) / dt
    return (position - oldPosition) / dt;
}