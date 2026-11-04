#pragma once

#include <string>
#include <utility>
#include <vector>

#include "core/Scene.h"
#include "physics/PhysicsWorld.h"

class AABBScene : public Scene
{
public:
    const char *getName() const override { return "AABB"; }

    void onEnter() override;
    void onUpdate(float fixedDt) override;
    void onRender(Renderer &renderer) override;

private:
    struct OverlapCheck
    {
        std::string label;
        bool expected;
        bool actual;
    };

    void checkOverlap(const char *label, const AABB &a, const AABB &b, bool expected);

    PhysicsWorld world;

    // Non-owning; the world owns every body.
    std::vector<RigidBody *> boxes;
    std::vector<std::pair<RigidBody *, RigidBody *>> circlePairs;

    // Results of the known-case AABB::overlaps checks and the rotation
    // check, built in onEnter and shown in the UI.
    std::vector<OverlapCheck> overlapChecks;
    Vec2 flatSize;
    Vec2 turnedSize;
};
