#pragma once

#include "math/Vec2.h"
#include "physics/ForceGenerator.h"

class GravityGenerator : public ForceGenerator
{
public:
    explicit GravityGenerator(Vec2 gravity);

    void updateForce(IForceReceiver &body, float dt) override;

private:
    Vec2 gravity;
};
