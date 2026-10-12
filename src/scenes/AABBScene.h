#pragma once

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
    PhysicsWorld world;

    // Non-owning; the world owns every body.
    std::vector<RigidBody *> boxes;
    std::vector<std::pair<RigidBody *, RigidBody *>> circlePairs;
};
