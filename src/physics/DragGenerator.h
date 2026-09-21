#pragma once

#include "physics/ForceGenerator.h"

class DragGenerator : public ForceGenerator
{
public:
    explicit DragGenerator(float dragCoefficient);

    void updateForce(IForceReceiver &body, float dt) override;

private:
    float dragCoefficient;
};
