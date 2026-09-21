#include "physics/DragGenerator.h"

DragGenerator::DragGenerator(float dragCoefficient)
    : dragCoefficient(dragCoefficient)
{
}

void DragGenerator::updateForce(IForceReceiver &body, float dt)
{
    Vec2 velocity = body.getVelocity(dt);

    float speed = velocity.length();

    if (speed <= 0.0001f)
        return;

    Vec2 dragForce = velocity * (-dragCoefficient * speed);

    body.applyForce(dragForce);
}
