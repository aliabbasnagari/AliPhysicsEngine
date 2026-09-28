#pragma once

#include "physics/ForceGenerator.h"

enum class DragMode
{
    Linear,
    Quadratic
};

class DragGenerator : public ForceGenerator
{
public:
    explicit DragGenerator(
        float coefficient = 0.5f,
        DragMode mode = DragMode::Linear);

    void updateForce(IForceReceiver &receiver, float dt) override;

    void setCoefficient(float coefficient) { this->coefficient = coefficient; }
    float getCoefficient() const { return coefficient; }

    void setMode(DragMode mode) { this->mode = mode; }
    DragMode getMode() const { return mode; }

private:
    float coefficient;
    DragMode mode;
};