#pragma once

#include "physics/Particle.h"

// Position-based correction: moves both particles directly so their
// separation equals targetDistance, splitting the move by inverse-
// mass share (invMass / (invMassA + invMassB)) so heavier particles
// move less. This is NOT a force - it never touches forceAccumulator
// or velocity, and it must run after integrate(), never during the
// force-accumulator pass from Task 3-5.
void solveDistanceConstraint(
    Particle &particleA,
    Particle &particleB,
    float targetDistance);

// Stateful wrapper so PhysicsWorld can hold a list of these, the same
// way it holds ForceGenerators and springs, and solve them once after
// every step(). Task 4-3 turns the single solve() call in step() into
// a small loop over N iterations; nothing here changes for that.
class DistanceConstraint
{
public:
    DistanceConstraint(
        Particle &particleA,
        Particle &particleB,
        float targetDistance);

    void solve();

    void setTargetDistance(float targetDistance)
    {
        this->targetDistance = targetDistance;
    }

    float getTargetDistance() const { return targetDistance; }

private:
    Particle &particleA;
    Particle &particleB;

    float targetDistance;
};