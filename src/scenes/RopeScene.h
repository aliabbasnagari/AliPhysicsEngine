#pragma once

#include <vector>

#include "core/Scene.h"
#include "physics/DistanceConstraint.h"
#include "physics/DragGenerator.h"
#include "physics/GravityGenerator.h"
#include "physics/PhysicsWorld.h"

class RopeScene : public Scene
{
public:
    const char *getName() const override { return "Rope"; }

    void onEnter() override;
    void onUpdate(float fixedDt) override;
    void onRender(Renderer &renderer) override;

private:
    void rebuildRope();

    PhysicsWorld world;

    // Non-owning; the world owns both lists. Kept here so the scene
    // can reach the anchor, the free end, and every constraint's
    // target distance for the sliders below.
    std::vector<VerletParticle *> ropeParticles;
    std::vector<VerletDistanceConstraint *> ropeConstraints;

    int particleCount = 15;
    float segmentLength = 0.3f;
    float particleMass = 0.2f;

    Vec2 anchorPosition = Vec2(0.0f, 3.0f);

    int constraintIterations = 4;
};