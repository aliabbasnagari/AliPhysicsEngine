#include "scenes/FallingBodiesScene.h"

#include <memory>

#include "graphics/Renderer.h"
#include "physics/GravityGenerator.h"
#include "scenes/BodyDrawing.h"

namespace
{
    constexpr float REMOVE_AT_SECONDS = 0.7f;
    constexpr float RESET_AT_SECONDS = 2.5f;
}

void FallingBodiesScene::onEnter()
{
    // Re-entrant: this object outlives each visit to the scene.
    world.clear();
    elapsed = 0.0f;

    world.addForceGenerator(
        std::make_unique<GravityGenerator>(Vec2(0.0f, -9.81f)));

    // Different shapes and masses: all must fall at the same rate.
    world.addRigidBody(RigidBody::createCircle(Vec2(-7.0f, 6.0f), 0.4f, 0.5f));
    world.addRigidBody(RigidBody::createBox(Vec2(-5.0f, 6.0f), Vec2(0.6f, 0.3f), 1.0f));
    removable = world.addRigidBody(RigidBody::createCircle(Vec2(-3.0f, 6.0f), 0.7f, 2.0f));
    world.addRigidBody(RigidBody::createBox(Vec2(-1.0f, 6.0f), Vec2(0.4f, 0.8f), 5.0f));

    // Already spinning: gravity acts at the centre of mass, so the spin
    // rate must stay constant while it falls.
    RigidBody *spinner = world.addRigidBody(
        RigidBody::createBox(Vec2(1.0f, 6.0f), Vec2(0.5f, 0.5f), 1.0f));
    spinner->angularVelocity = 3.0f;
    spinner->rotation = 0.3f;

    // Sideways velocity.
    RigidBody *drifter = world.addRigidBody(
        RigidBody::createCircle(Vec2(3.0f, 6.0f), 0.5f, 1.0f));
    drifter->linearVelocity = Vec2(1.5f, 0.0f);

    // Static (mass 0): gravity must not move it.
    RigidBody *anchor = world.addRigidBody(
        RigidBody::createBox(Vec2(6.0f, 2.0f), Vec2(0.8f, 0.3f), 0.0f));
    anchor->rotation = 0.4f;
}

void FallingBodiesScene::onUpdate(float fixedDt)
{
    world.step(fixedDt);

    elapsed += fixedDt;

    if (removable != nullptr && elapsed > REMOVE_AT_SECONDS)
    {
        world.removeRigidBody(removable);
        removable = nullptr; // the pointer is dead now
    }

    // Bodies fall off screen (no collisions yet); restart the demo.
    if (elapsed > RESET_AT_SECONDS)
    {
        onEnter();
    }
}

void FallingBodiesScene::onRender(Renderer &renderer)
{
    for (const auto &body : world.getBodies())
    {
        drawBody(renderer, *body);
    }
}
