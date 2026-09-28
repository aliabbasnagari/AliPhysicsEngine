#pragma once

#include <deque>

#include "core/Scene.h"
#include "physics/DistanceConstraint.h"
#include "physics/GravityGenerator.h"
#include "physics/PhysicsWorld.h"
#include "physics/SpringForceGenerator.h"

#include "graphics/Renderer.h"

class DistanceConstraintScene : public Scene
{
public:
    const char *getName() const override { return "Distance Constraint"; }

    void onEnter() override;
    void onUpdate(float fixedDt) override;
    void onRender(Renderer &renderer) override;

private:
    void reset();

    void drawTrail(
        Renderer &renderer,
        const std::deque<Vec2> &trailPoints,
        const Color &color) const;

    // --- Pendulum comparison: constraint vs spring, each with a
    // fixed anchor (inverseMass == 0) and one free particle, under
    // gravity. ---
    PhysicsWorld pendulumWorld;

    Particle *constraintAnchor = nullptr;
    Particle *constraintFree = nullptr;
    DistanceConstraint *pendulumConstraint = nullptr;

    Particle *springAnchor = nullptr;
    Particle *springFree = nullptr;
    SpringForceGenerator *pendulumSpring = nullptr;

    Vec2 constraintAnchorPosition = Vec2(-3.5f, 2.5f);
    Vec2 springAnchorPosition = Vec2(-0.5f, 2.5f);

    Vec2 constraintAnchorStartPosition;

    float pendulumLength = 1.6f;
    float springStiffness = 40.0f;
    float springDamping = 0.2f;

    std::deque<Vec2> constraintTrail;
    std::deque<Vec2> springTrail;
    static constexpr std::size_t trailLength = 240;

    static constexpr int historyLength = 300; // ~5s at 60Hz
    float constraintDistanceHistory[historyLength] = {};
    float springDistanceHistory[historyLength] = {};
    int historyOffset = 0;

    // --- Free-free pair: equal mass, no gravity, isolating the
    // constraint's 50/50 split with nothing else able to move them. ---
    PhysicsWorld freeFreeWorld;

    Particle *freeA = nullptr;
    Particle *freeB = nullptr;
    DistanceConstraint *freeFreeConstraint = nullptr;

    Vec2 freeAStartPosition = Vec2(2.5f, -1.0f);
    Vec2 freeBStartPosition = Vec2(5.0f, -1.0f); // separation 2.5

    float freeFreeTargetDistance = 1.2f;
    float freeMass = 1.0f;

    float lastFreeADelta = 0.0f;
    float lastFreeBDelta = 0.0f;
};