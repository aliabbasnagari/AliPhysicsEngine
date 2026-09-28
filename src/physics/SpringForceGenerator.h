#pragma once

#include "physics/IForceReceiver.h"

// Two-body spring. Unlike ForceGenerator (which acts on a single
// receiver via updateForce(IForceReceiver&, dt)), a spring needs both
// endpoints in the same calculation — relative velocity for damping,
// and equal-and-opposite application per Newton's third law. It does
// not implement ForceGenerator; PhysicsWorld keeps it in its own list
// and calls updateForce(dt) once per step.
class SpringForceGenerator
{
public:
    SpringForceGenerator(
        IForceReceiver &particleA,
        IForceReceiver &particleB,
        float stiffness,
        float restLength,
        float damping = 0.0f);

    // Computes the spring+damping force once and applies it to both
    // endpoints (opposite signs) in a single call.
    void updateForce(float dt);

    void setStiffness(float stiffness) { this->stiffness = stiffness; }
    float getStiffness() const { return stiffness; }

    void setRestLength(float restLength) { this->restLength = restLength; }
    float getRestLength() const { return restLength; }

    void setDamping(float damping) { this->damping = damping; }
    float getDamping() const { return damping; }

private:
    IForceReceiver &particleA;
    IForceReceiver &particleB;

    float stiffness;
    float restLength;
    float damping;
};