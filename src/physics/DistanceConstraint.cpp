#include "physics/DistanceConstraint.h"

#include <cmath>

void solveDistanceConstraint(
    Particle &particleA,
    Particle &particleB,
    float targetDistance)
{
    Vec2 delta = particleB.position - particleA.position; // A -> B

    float currentDistance =
        std::sqrt(delta.x * delta.x + delta.y * delta.y);

    // Degenerate: particles coincide, direction is undefined.
    if (currentDistance < 1e-6f)
    {
        return;
    }

    float totalInverseMass = particleA.inverseMass + particleB.inverseMass;

    // Both infinite mass - neither can move, nothing to solve.
    if (totalInverseMass <= 0.0f)
    {
        return;
    }

    Vec2 direction = delta * (1.0f / currentDistance);

    // Positive when too far apart, negative when too close.
    float error = currentDistance - targetDistance;

    Vec2 correction = direction * error;

    float shareA = particleA.inverseMass / totalInverseMass;
    float shareB = particleB.inverseMass / totalInverseMass;

    // Too far apart: A moves toward B, B moves toward A.
    // Too close: both signs flip and they push apart instead.
    // inverseMass == 0 -> share == 0 -> that particle never moves,
    // regardless of what the other particle's mass is.
    particleA.position = particleA.position + correction * shareA;
    particleB.position = particleB.position - correction * shareB;
}

DistanceConstraint::DistanceConstraint(
    Particle &particleA,
    Particle &particleB,
    float targetDistance)
    : particleA(particleA),
      particleB(particleB),
      targetDistance(targetDistance)
{
}

void DistanceConstraint::solve()
{
    solveDistanceConstraint(particleA, particleB, targetDistance);
}