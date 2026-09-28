#pragma once

#include <deque>

#include "core/Scene.h"
#include "physics/GravityGenerator.h"
#include "physics/PhysicsWorld.h"
#include "physics/SpringForceGenerator.h"

#include "graphics/Renderer.h"

class HookeSpringScene : public Scene
{
public:
    const char *getName() const override { return "Hooke's Law Spring"; }

    void onEnter() override;
    void onUpdate(float fixedDt) override;
    void onRender(Renderer &renderer) override;

private:
    void reset();

    void drawTrail(
        Renderer &renderer,
        const std::deque<Vec2> &trailPoints,
        const Color &color) const;

    float computeTotalEnergy() const;

    PhysicsWorld world;

    // inverseMass == 0 — the fixed end of the spring.
    Particle *anchorParticle = nullptr;

    // Hangs under gravity + the spring.
    Particle *freeParticle = nullptr;

    // Non-owning; PhysicsWorld owns both, returned by addForceGenerator
    // / addSpring so sliders can keep tuning the live objects.
    GravityGenerator *gravityGenerator = nullptr;
    SpringForceGenerator *spring = nullptr;

    Vec2 anchorPosition = Vec2(0.0f, 2.5f);
    Vec2 freeStartPosition = Vec2(2.0f, 2.5f); // horizontal at start

    Vec2 gravity = Vec2(0.0f, -9.81f);
    float freeMass = 1.0f;

    float stiffness = 20.0f;
    float restLength = 1.5f;
    float damping = 0.0f;

    int integrationModeIndex = 1; // 0 = Explicit, 1 = SemiImplicit

    float lastFixedDt = 1.0f / 60.0f;

    static constexpr std::size_t trailLength = 300;
    std::deque<Vec2> trail;

    static constexpr int energyHistoryLength = 300; // ~5s at 60Hz
    float energyHistory[energyHistoryLength] = {};
    int energyHistoryOffset = 0;
};