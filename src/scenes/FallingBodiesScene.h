#pragma once

#include "core/Scene.h"
#include "physics/PhysicsWorld.h"

class FallingBodiesScene : public Scene
{
public:
    const char *getName() const override { return "Falling Bodies"; }

    void onEnter() override;
    void onUpdate(float fixedDt) override;
    void onRender(Renderer &renderer) override;

private:
    PhysicsWorld world;

    // Non-owning; removed from the world partway through to prove that
    // removal doesn't disturb the other bodies.
    RigidBody *removable = nullptr;
    float elapsed = 0.0f;
};
