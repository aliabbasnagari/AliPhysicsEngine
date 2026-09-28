#include "physics/DragGenerator.h"

#include <cmath>

DragGenerator::DragGenerator(float coefficient, DragMode mode)
    : coefficient(coefficient),
      mode(mode)
{
}

void DragGenerator::updateForce(IForceReceiver &receiver, float dt)
{
    Vec2 velocity = receiver.getVelocity(dt);

    float speed =
        std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y);

    // Nothing to oppose at rest, and it avoids doing pointless work
    // in the quadratic branch below.
    if (speed < 1e-6f)
    {
        return;
    }

    Vec2 force;

    if (mode == DragMode::Linear)
    {
        // F = -k * v
        force = velocity * -coefficient;
    }
    else
    {
        // F = -k * |v| * v. Since v already equals speed * direction,
        // multiplying the whole vector by speed gives |v| * v in one
        // step — no separate normalize() call needed. Writing this as
        // velocity * velocity instead would square each component,
        // which isn't a defined vector operation and isn't what the
        // physics means by "squared" here.
        force = velocity * (-coefficient * speed);
    }

    receiver.applyForce(force);
}