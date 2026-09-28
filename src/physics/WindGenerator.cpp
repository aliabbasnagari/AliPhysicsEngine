#include "physics/WindGenerator.h"

WindGenerator::WindGenerator(Vec2 force)
    : force(force)
{
}

void WindGenerator::updateForce(IForceReceiver &receiver, float dt)
{
    (void)dt;
    receiver.applyForce(force);
}