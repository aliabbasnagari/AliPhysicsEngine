#pragma once

#include "math/Vec2.h"

class IForceReceiver
{
public:
    virtual ~IForceReceiver() = default;

    virtual void applyForce(Vec2 force) = 0;
    virtual float getMass() const = 0;
    virtual float getInverseMass() const = 0;
    virtual Vec2 getVelocity(float dt) const = 0;
};
