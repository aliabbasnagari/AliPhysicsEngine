#pragma once

#include "core/Scene.h"
#include "physics/PhysicsWorld.h"

class Renderer;

class BroadPhaseScene : public Scene
{
public:
    const char *getName() const override { return "BroadPhase Scene"; }

    void onEnter() override;
    void onUpdate(float fixedDt) override;
    void onRender(Renderer &renderer) override;

private:
    PhysicsWorld world;
    float elapsed = 0.0f;

    bool pairsAreValid() const;
};