#pragma once

#include "core/Scene.h"
#include "physics/PhysicsWorld.h"

class BroadPhaseComparisonScene : public Scene
{
public:
    const char *getName() const override { return "BroadPhase Comparison"; }

    void onEnter() override;
    void onUpdate(float fixedDt) override;
    void onRender(Renderer &renderer) override;

private:
    PhysicsWorld world;
    std::vector<RigidBody *> bodies;
    bool drawGrid = true;
    float cellSize = 2.0f;
};
