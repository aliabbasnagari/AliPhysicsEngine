#pragma once

#include <deque>

#include "core/Scene.h"
#include "physics/DragGenerator.h"
#include "physics/GravityGenerator.h"
#include "physics/PhysicsWorld.h"
#include "physics/WindGenerator.h"

#include "graphics/Renderer.h"

class ForceAccumulatorScene : public Scene
{
public:
    const char *getName() const override
    {
        return "Force Accumulator Pattern";
    }

    void onEnter() override;
    void onUpdate(float fixedDt) override;
    void onRender(Renderer &renderer) override;

private:
    void relaunch();
    void rebuildGenerators();

    void drawTrail(
        Renderer &renderer,
        const std::deque<Vec2> &trailPoints,
        const Color &color) const;

    PhysicsWorld world;
    Particle *particle = nullptr;

    // Non-owning; re-captured every rebuildGenerators() call, since
    // clearForceGenerators() invalidates whatever was held before.
    GravityGenerator *gravityGenerator = nullptr;
    DragGenerator *dragGenerator = nullptr;
    WindGenerator *windGenerator = nullptr;

    bool gravityEnabled = true;
    bool dragEnabled = false;
    bool windEnabled = false;

    Vec2 gravity = Vec2(0.0f, -9.81f);
    float dragCoefficient = 0.6f;
    Vec2 windForce = Vec2(2.0f, 0.0f);

    Vec2 launchPosition = Vec2(-3.0f, 2.0f);
    Vec2 launchVelocity = Vec2(2.0f, 3.0f);

    static constexpr std::size_t trailLength = 300;
    std::deque<Vec2> trail;
};