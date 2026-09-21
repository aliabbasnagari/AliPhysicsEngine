#pragma once

#include "physics/IForceReceiver.h"

class ForceGenerator
{
public:
    virtual ~ForceGenerator() = default;

    virtual void updateForce(IForceReceiver &body, float dt) = 0;
};
