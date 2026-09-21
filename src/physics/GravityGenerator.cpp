#include "physics/GravityGenerator.h"

GravityGenerator::GravityGenerator(Vec2 gravity)
    : gravity(gravity)
{
}

void GravityGenerator::updateForce(IForceReceiver &body, float /*dt*/)
{
    if (body.getInverseMass() == 0.0f)
        return;

    Vec2 force = gravity * body.getMass();

    body.applyForce(force);
}
