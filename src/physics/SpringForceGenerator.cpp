#include "physics/SpringForceGenerator.h"

#include <cmath>

SpringForceGenerator::SpringForceGenerator(
    IForceReceiver &particleA,
    IForceReceiver &particleB,
    float stiffness,
    float restLength,
    float damping)
    : particleA(particleA),
      particleB(particleB),
      stiffness(stiffness),
      restLength(restLength),
      damping(damping)
{
}

void SpringForceGenerator::updateForce(float dt)
{
    Vec2 positionA = particleA.getPosition(dt);
    Vec2 positionB = particleB.getPosition(dt);

    // Points from B toward A. A positive magnitude below therefore
    // pulls A toward B — the restoring direction when stretched.
    Vec2 delta = positionA - positionB;

    float length = std::sqrt(delta.x * delta.x + delta.y * delta.y);

    // Degenerate case: the two particles coincide, so the spring axis
    // is undefined and there's nothing sensible to push along.
    if (length < 1e-6f)
    {
        return;
    }

    Vec2 direction = delta * (1.0f / length);

    float extension = length - restLength;

    // Hooke's law: F = -k * x. Stretched (extension > 0) gives a
    // negative magnitude — a pull on A back toward B.
    float magnitude = -stiffness * extension;

    if (damping > 0.0f)
    {
        Vec2 velocityA = particleA.getVelocity(dt);
        Vec2 velocityB = particleB.getVelocity(dt);

        Vec2 relativeVelocity = velocityA - velocityB;

        // Rate at which the spring is currently stretching, projected
        // onto its axis. Damping opposes exactly this component,
        // which is what removes energy without fighting the restoring
        // force itself.
        float stretchRate =
            relativeVelocity.x * direction.x +
            relativeVelocity.y * direction.y;

        magnitude -= damping * stretchRate;
    }

    Vec2 forceOnA = direction * magnitude;
    Vec2 forceOnB = forceOnA * -1.0f; // Newton's third law

    particleA.applyForce(forceOnA);
    particleB.applyForce(forceOnB);
}