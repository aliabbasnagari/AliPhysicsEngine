#pragma once

#include "core/Scene.h"
#include "physics/PhysicsWorld.h"

class GravityScene : public Scene
{
public:
    const char *getName() const override { return "Gravity - Explicit Euler"; }

    void onEnter() override;

    void onUpdate(float fixedDt) override;
    void onRender(Renderer &renderer) override;

private:
    PhysicsWorld world;
};