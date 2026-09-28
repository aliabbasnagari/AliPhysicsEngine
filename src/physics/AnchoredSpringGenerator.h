#pragma once

#include "physics/Particle.h"
#include "physics/ForceGenerator.h"

class AnchoredSpringGenerator : public ForceGenerator
{
public:
    AnchoredSpringGenerator(
        const Vec2 &anchor,
        float restLength,
        float springConstant);

    void updateForce(IForceReceiver &receiver, float dt) override;

private:
    Vec2 anchor;

    float restLength;
    float springConstant;
};