#pragma once

#include "physics/ForceGenerator.h"

// A constant force, independent of mass and velocity — the simplest
// possible generator. It exists purely to prove that a brand-new
// force needs nothing beyond this file plus one addForceGenerator
// call.
class WindGenerator : public ForceGenerator
{
public:
    explicit WindGenerator(Vec2 force = Vec2(0.0f, 0.0f));

    void updateForce(IForceReceiver &receiver, float dt) override;

    void setForce(Vec2 force) { this->force = force; }
    Vec2 getForce() const { return force; }

private:
    Vec2 force;
};