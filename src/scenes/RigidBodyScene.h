#pragma once

#include <vector>

#include "core/Scene.h"
#include "physics/PhysicsWorld.h"

// A constant load re-applied to one body every step.
struct BodyLoad
{
    RigidBody *body;  // non-owning; the world owns it
    Vec2 force;       // world-space force
    Vec2 localPoint;  // application point in body space
    float torque;
};

class RigidBodyScene : public Scene
{
public:
    const char *getName() const override { return "Rigid Body Data"; }

    void onEnter() override;
    void onUpdate(float fixedDt) override;
    void onRender(Renderer &renderer) override;

private:
    void addTest(RigidBody body, Vec2 force, Vec2 localPoint, float torque);

    PhysicsWorld world;
    std::vector<BodyLoad> loads;
    float elapsed = 0.0f;
};
