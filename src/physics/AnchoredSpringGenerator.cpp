#include "physics/AnchoredSpringGenerator.h"

#include <cmath>

AnchoredSpringGenerator::AnchoredSpringGenerator(
    const Vec2 &anchor,
    float restLength,
    float springConstant)
    : anchor(anchor),
      restLength(restLength),
      springConstant(springConstant)
{
}

void AnchoredSpringGenerator::updateForce(IForceReceiver &receiver, float dt)
{
    (void)dt;

    Vec2 delta = receiver.getPosition(dt) - anchor;

    float length =
        std::sqrt(delta.x * delta.x + delta.y * delta.y);

    // Degenerate case: the particle sits on the anchor, so the
    // spring direction is undefined.
    if (length < 1e-6f)
    {
        return;
    }

    float extension = length - restLength;

    float magnitude = -springConstant * extension;

    Vec2 direction = delta * (1.0f / length);

    receiver.applyForce(direction * magnitude);
}