#include "physics/VerletParticle.h"

VerletParticle::VerletParticle(
    Vec2 position,
    Vec2 velocity,
    float mass,
    float fixedDt)
    : position(position),
      oldPosition(position - velocity * fixedDt),
      mass(mass),
      inverseMass(mass > 0.0f ? 1.0f / mass : 0.0f)
{
}

void VerletParticle::integrate(float dt, Vec2 acceleration)
{
    // inverseMass == 0 means infinite mass / static particle.
    if (inverseMass == 0.0f)
        return;

    Vec2 currentPosition = position;

    position =
        2.0f * position - oldPosition + acceleration * (dt * dt);

    oldPosition = currentPosition;
}

Vec2 VerletParticle::getVelocity(float dt) const
{
    if (dt <= 0.0f)
        return Vec2(0.0f, 0.0f);

    return (position - oldPosition) / dt;
}