#pragma once

#include <vector>

#include "core/Scene.h"
#include "physics/RigidBody.h"

class RigidBodyScene : public Scene
{
public:
    const char *getName() const override { return "Rigid Body Data"; }
    void onUpdate(float fixedDt) override;
    void onEnter() override;
    void onRender(Renderer &renderer) override;

private:
    std::vector<RigidBody> rigidBodies;
};
