#pragma once

#include <deque>

#include "core/Scene.h"
#include "physics/DragGenerator.h"
#include "physics/PhysicsWorld.h"

#include "graphics/Renderer.h"

class DragScene : public Scene
{
public:
    const char *getName() const override { return "Drag / Damping"; }

    void onEnter() override;
    void onUpdate(float fixedDt) override;
    void onRender(Renderer &renderer) override;

private:
    void relaunch();

    void drawTrail(
        Renderer &renderer,
        const std::deque<Vec2> &trail,
        const Color &color) const;

    PhysicsWorld world;
    Particle *particle = nullptr;

    // Non-owning; PhysicsWorld owns the actual instance. Registered
    // through addForceGenerator, so world.step() runs it — sliders
    // just call setters on the same live object.
    DragGenerator *dragGenerator = nullptr;

    float dragCoefficient = 1.0f;
    DragMode dragMode = DragMode::Linear;

    Vec2 launchPosition = Vec2(-3.0f, 0.0f);
    float launchSpeed = 3.0f;
    Vec2 launchVelocity = Vec2(3.0f, 0.0f);
    Vec2 launchDirection = Vec2(1.0f, 0.0f);

    bool everReversed = false;
    bool everIncreased = false;

    static constexpr std::size_t trailLength = 300;
    std::deque<Vec2> trail;

    static constexpr int speedHistoryLength = 300;
    float speedHistory[speedHistoryLength] = {};
    int speedHistoryOffset = 0;
};