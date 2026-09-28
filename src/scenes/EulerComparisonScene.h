#pragma once

#include <deque>

#include "core/Scene.h"
#include "physics/PhysicsWorld.h"

#include "graphics/Renderer.h"

class EulerComparisonScene : public Scene
{
public:
    const char *getName() const override { return "Explicit vs Semi-Implicit Euler"; }

    void onEnter() override;

    void onUpdate(float fixedDt) override;
    void onRender(Renderer &renderer) override;

private:
    void drawTrail(
        Renderer &renderer,
        const std::deque<Vec2> &trail,
        const Color &color) const;

    float computeEnergy(const Particle &particle) const;

    PhysicsWorld world;

    Particle *explicitParticle = nullptr;
    Particle *semiImplicitParticle = nullptr;

    Vec2 anchor = Vec2(0.0f, 0.0f);
    float springConstant = 8.0f;
    float initialDisplacement = 2.0f;

    static constexpr std::size_t trailLength = 180; // ~3s at 60Hz
    std::deque<Vec2> explicitTrail;
    std::deque<Vec2> semiImplicitTrail;

    // Fixed-size ring buffers for ImGui::PlotLines (values_offset
    // pattern — no per-frame reallocation).
    static constexpr int energyHistoryLength = 300; // ~5s
    float explicitEnergyHistory[energyHistoryLength] = {};
    float semiImplicitEnergyHistory[energyHistoryLength] = {};
    int energyHistoryOffset = 0;
};