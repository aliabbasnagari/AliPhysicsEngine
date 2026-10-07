#pragma once

#include <vector>

#include "core/Scene.h"
#include "physics/RigidBody.h"

class RigidBodyScene : public Scene
{
public:
    const char *getName() const override { return "Rigid Body Data"; }

    void onEnter() override;
    void onUpdate(float fixedDt) override;
    void onRender(Renderer &renderer) override;

private:
    // A constant load re-applied to one body every step.
    struct Push
    {
        size_t body;
        Vec2 force;       // world-space force
        Vec2 localPoint;  // application point in body space
        float torque;
    };

    void addTest(const RigidBody &body, Vec2 force, Vec2 localPoint, float torque);

    std::vector<RigidBody> rigidBodies;
    std::vector<Push> pushes;
    float elapsed = 0.0f;
};
