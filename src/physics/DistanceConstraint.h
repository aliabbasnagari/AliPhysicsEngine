#pragma once

#include <cmath>

#include "math/Vec2.h"
#include "physics/Particle.h"
#include "physics/VerletParticle.h"

// Position-based correction: moves both particles directly so their
// separation equals targetDistance, splitting the move by inverse-
// mass share. Templated because the only things this math touches —
// public `position` and `inverseMass` — are shaped identically on
// Particle and VerletParticle. That's what lets a rope reuse this
// exact solver over VerletParticle chains instead of duplicating it.
template <typename ParticleT>
void solveDistanceConstraint(
    ParticleT &particleA,
    ParticleT &particleB,
    float targetDistance)
{
    Vec2 delta = particleB.position - particleA.position; // A -> B

    float currentDistance =
        std::sqrt(delta.x * delta.x + delta.y * delta.y);

    // Degenerate: particles coincide, direction undefined.
    if (currentDistance < 1e-6f)
    {
        return;
    }

    float totalInverseMass = particleA.inverseMass + particleB.inverseMass;

    // Both infinite mass - nothing can move.
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

    // inverseMass == 0 -> share == 0 -> that particle never moves,
    // regardless of the other's mass. This is how the rope's anchor
    // stays fixed through every constraint pass.
    particleA.position = particleA.position + correction * shareA;
    particleB.position = particleB.position - correction * shareB;
}

// Stateful wrapper so a world can hold a list and re-solve them
// repeatedly per step - the Gauss-Seidel relaxation this task is
// about. Solved in the order they were added, so a sweep that starts
// at the anchor and runs toward the free end propagates corrections
// outward within a single iteration, not just across iterations.
template <typename ParticleT>
class DistanceConstraintT
{
public:
    DistanceConstraintT(
        ParticleT &particleA,
        ParticleT &particleB,
        float targetDistance)
        : particleA(particleA),
          particleB(particleB),
          targetDistance(targetDistance)
    {
    }

    void solve()
    {
        solveDistanceConstraint(particleA, particleB, targetDistance);
    }

    void setTargetDistance(float targetDistance)
    {
        this->targetDistance = targetDistance;
    }

    float getTargetDistance() const { return targetDistance; }

private:
    ParticleT &particleA;
    ParticleT &particleB;

    float targetDistance;
};

// Task 4-2's exact type, now spelled as an alias. Every existing
// call site - PhysicsWorld's addConstraint/constraints,
// DistanceConstraintScene - compiles against this with no changes.
using DistanceConstraint = DistanceConstraintT<Particle>;

// Task 4-3: the same solver, instantiated for VerletParticle chains.
using VerletDistanceConstraint = DistanceConstraintT<VerletParticle>;