#include "scenes/BroadPhaseScene.h"

#include <algorithm>
#include <set>
#include <utility>

#include "graphics/Renderer.h"
#include "imgui.h"
#include "scenes/BodyDrawing.h"

#include "physics/BroadPhase.h"

namespace
{
    constexpr float PI = 3.14159265358979f;
    constexpr float RESET_SECONDS = 8.0f;

    RigidBody *addCircle(PhysicsWorld &world, Vec2 position, float radius, Vec2 velocity)
    {
        RigidBody *body = world.addRigidBody(RigidBody::createCircle(position, radius, 1.0f));
        body->linearVelocity = velocity;
        return body;
    }

    RigidBody *addBox(PhysicsWorld &world, Vec2 position, Vec2 half, float rotation, Vec2 velocity)
    {
        RigidBody *body = world.addRigidBody(RigidBody::createBox(position, half, 1.0f));
        body->rotation = rotation;
        body->linearVelocity = velocity;
        return body;
    }
}

void BroadPhaseScene::onEnter()
{
    // Re-entrant: this object outlives each visit to the scene.
    world.clear();
    elapsed = 0.0f;

    // No gravity: bodies just drift, so pairs form and break by motion alone.

    // Two groups moving through each other: the count rises, then falls.
    addCircle(world, Vec2(-8.0f, 3.0f), 0.6f, Vec2(1.5f, 0.0f));
    addBox(world, Vec2(-9.5f, 3.4f), Vec2(0.7f, 0.4f), 0.3f, Vec2(1.5f, 0.0f));
    addCircle(world, Vec2(-8.5f, 4.5f), 0.5f, Vec2(1.5f, 0.0f));

    addCircle(world, Vec2(8.0f, 3.0f), 0.6f, Vec2(-1.5f, 0.0f));
    addBox(world, Vec2(9.5f, 3.2f), Vec2(0.7f, 0.4f), PI / 4.0f, Vec2(-1.5f, 0.0f));
    addCircle(world, Vec2(8.5f, 1.8f), 0.5f, Vec2(-1.5f, 0.0f));

    // Loose cluster in the middle that overlaps from the start: a chain
    // where only neighbours touch (expect 2 pairs among these three).
    addCircle(world, Vec2(-1.2f, -2.0f), 0.6f, Vec2(0.0f, 0.0f));
    addCircle(world, Vec2(0.0f, -2.0f), 0.6f, Vec2(0.0f, 0.0f));
    addCircle(world, Vec2(1.2f, -2.0f), 0.6f, Vec2(0.0f, 0.0f));

    // Far from everything: must never pair.
    addCircle(world, Vec2(-10.0f, -5.5f), 0.5f, Vec2(0.0f, 0.0f));
    addBox(world, Vec2(10.0f, -5.5f), Vec2(0.5f, 0.5f), 0.0f, Vec2(0.0f, 0.0f));

    // Two overlapping static bodies: must be skipped (static-static).
    RigidBody *s1 = world.addRigidBody(RigidBody::createBox(Vec2(-5.0f, -5.0f), Vec2(1.0f, 0.4f), 0.0f));
    RigidBody *s2 = world.addRigidBody(RigidBody::createBox(Vec2(-4.2f, -5.0f), Vec2(1.0f, 0.4f), 0.0f));
    s2->rotation = 0.5f;
    (void)s1;

    // Static + dynamic overlap: must be kept.
    world.addRigidBody(RigidBody::createBox(Vec2(5.0f, -5.0f), Vec2(1.0f, 0.4f), 0.0f));
    addCircle(world, Vec2(5.2f, -4.7f), 0.5f, Vec2(0.0f, 0.0f));
}

void BroadPhaseScene::onUpdate(float fixedDt)
{
    world.step(fixedDt);

    elapsed += fixedDt;
    if (elapsed > RESET_SECONDS)
    {
        onEnter();
    }
}

bool BroadPhaseScene::pairsAreValid() const
{
    std::set<std::pair<const RigidBody *, const RigidBody *>> seen;
    for (const BodyPair &pair : world.getCandidatePairs())
    {
        if (pair.a == pair.b)
        {
            return false;
        }
        if (pair.a->isStatic() && pair.b->isStatic())
        {
            return false;
        }
        // (A,B) and (B,A) are the same pair.
        const auto key = std::minmax<const RigidBody *>(pair.a, pair.b);
        if (!seen.insert({key.first, key.second}).second)
        {
            return false;
        }
    }
    return true;
}

void BroadPhaseScene::onRender(Renderer &renderer)
{
    for (const auto &body : world.getBodies())
    {
        drawBody(renderer, *body);
        drawAABB(renderer, body->computeAABB(), Color(0.3f, 0.8f, 0.3f));
    }

    // Red over green for every body that is part of a candidate pair.
    for (const BodyPair &pair : world.getCandidatePairs())
    {
        drawAABB(renderer, pair.a->computeAABB(), Color(1.0f, 0.3f, 0.3f));
        drawAABB(renderer, pair.b->computeAABB(), Color(1.0f, 0.3f, 0.3f));
    }

    ImGui::Begin("Broad Phase");
    ImGui::Text("Bodies: %zu", world.getBodies().size());
    ImGui::Text("Candidate pairs: %zu", world.getCandidatePairs().size());
    ImGui::Text("Pair list valid: %s", pairsAreValid() ? "yes" : "NO");
    ImGui::End();
}