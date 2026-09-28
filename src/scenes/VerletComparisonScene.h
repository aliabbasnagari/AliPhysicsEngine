#pragma once

#include <deque>

#include "core/Scene.h"
#include "physics/PhysicsWorld.h"

#include "graphics/Renderer.h"

class VerletComparisonScene : public Scene
{
public:
    const char *getName() const override
    {
        return "Gravity - Verlet vs Semi-Implicit Euler";
    }

    void onEnter() override;

    void onUpdate(float fixedDt) override;
    void onRender(Renderer &renderer) override;

private:
    void drawTrail(
        Renderer &renderer,
        const std::deque<Vec2> &trail,
        const Color &color) const;

    PhysicsWorld world;

    Particle *semiImplicitParticle = nullptr;
    VerletParticle *verletParticle = nullptr;

    Vec2 startPosition = Vec2(-2.0f, 3.0f);
    Vec2 startVelocity = Vec2(1.5f, 0.0f);

    float lastFixedDt = 1.0f / 60.0f;

    static constexpr std::size_t trailLength = 240; // ~4s at 60Hz
    std::deque<Vec2> semiImplicitTrail;
    std::deque<Vec2> verletTrail;

    static constexpr int historyLength = 300; // ~5s
    float positionErrorHistory[historyLength] = {};
    float velocityErrorHistory[historyLength] = {};
    int historyOffset = 0;

    // Y-level used by the constraint-snap demo below.
    float snapTargetY = -3.0f;
};