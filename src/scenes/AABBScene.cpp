#include "scenes/AABBScene.h"

#include "graphics/Renderer.h"
#include "scenes/BodyDrawing.h"

namespace
{
    constexpr float PI = 3.14159265358979f;

    void checkOverlap(const char *label, const AABB &a, const AABB &b, bool expected)
    {
        const bool actual = a.overlaps(b);
    }
}

void AABBScene::onEnter()
{
    // Re-entrant: this object outlives each visit to the scene.
    world.clear();
    boxes.clear();
    circlePairs.clear();

    // No gravity: this scene is only about the bounds.
    const Vec2 half(1.0f, 0.5f);

    // Static boxes at increasing rotation. At pi/4 the AABB is largest.
    const float angles[] = {0.0f, PI / 8.0f, PI / 4.0f, 3.0f * PI / 8.0f, PI / 2.0f};
    for (int i = 0; i < 5; ++i)
    {
        RigidBody *box = world.addRigidBody(
            RigidBody::createBox(Vec2(-7.0f + 2.8f * i, 3.0f), half, 0.0f));
        box->rotation = angles[i];
        boxes.push_back(box);
    }

    // Spinning box: its AABB should pulse as it rotates.
    RigidBody *spinner = world.addRigidBody(
        RigidBody::createBox(Vec2(7.5f, 3.0f), half, 1.0f));
    spinner->angularVelocity = 1.0f;
    boxes.push_back(spinner);

    // Circle pairs with radius 0.5. Centre gaps of 1.0, 1.05 and 0.95:
    // exactly touching, just apart, just overlapping.
    const float spacing[] = {1.0f, 1.05f, 0.95f};
    for (int i = 0; i < 3; ++i)
    {
        const float x = -6.0f + 4.0f * i;
        RigidBody *a = world.addRigidBody(RigidBody::createCircle(Vec2(x, -2.0f), 0.5f, 0.0f));
        RigidBody *b = world.addRigidBody(RigidBody::createCircle(Vec2(x + spacing[i], -2.0f), 0.5f, 0.0f));
        circlePairs.push_back({a, b});
    }

    // Known cases for AABB::overlaps.
    const AABB base{Vec2(0.0f, 0.0f), Vec2(2.0f, 2.0f)};
    checkOverlap("overlapping", base, AABB{Vec2(1.0f, 1.0f), Vec2(3.0f, 3.0f)}, true);
    checkOverlap("separated on x", base, AABB{Vec2(3.0f, 0.0f), Vec2(5.0f, 2.0f)}, false);
    checkOverlap("separated on y", base, AABB{Vec2(0.0f, 3.0f), Vec2(2.0f, 5.0f)}, false);
    checkOverlap("touching edge", base, AABB{Vec2(2.0f, 0.0f), Vec2(4.0f, 2.0f)}, true);
    checkOverlap("fully inside", base, AABB{Vec2(0.5f, 0.5f), Vec2(1.5f, 1.5f)}, true);

    // Rotation check: 45 degrees must be larger than 0.
    RigidBody flat = RigidBody::createBox(Vec2(0.0f, 0.0f), half, 1.0f);
    RigidBody turned = RigidBody::createBox(Vec2(0.0f, 0.0f), half, 1.0f);
    turned.rotation = PI / 4.0f;
    const Vec2 s0 = flat.computeAABB().size();
    const Vec2 s45 = turned.computeAABB().size();
}

void AABBScene::onUpdate(float fixedDt)
{
    world.step(fixedDt);
}

void AABBScene::onRender(Renderer &renderer)
{
    for (const auto &body : world.getBodies())
    {
        drawBody(renderer, *body);
    }

    for (const RigidBody *box : boxes)
    {
        drawAABB(renderer, box->computeAABB(), Color(1.0f, 0.9f, 0.2f));
    }

    // Red when the pair's AABBs overlap (touching counts), green otherwise.
    for (const auto &pair : circlePairs)
    {
        const AABB a = pair.first->computeAABB();
        const AABB b = pair.second->computeAABB();
        const Color color = a.overlaps(b) ? Color(1.0f, 0.3f, 0.3f) : Color(0.3f, 1.0f, 0.3f);
        drawAABB(renderer, a, color);
        drawAABB(renderer, b, color);
    }
}
