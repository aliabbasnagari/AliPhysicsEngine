#include "scenes/RigidBodyScene.h"

#include <cstdio>

#include "graphics/Renderer.h"

namespace
{
    constexpr float PI = 3.14159265358979f;
    constexpr float AXIS_LENGTH = 0.6f;
}

void RigidBodyScene::onEnter()
{
    // Re-entrant: this object outlives each visit to the scene.
    rigidBodies.clear();

    const Vec2 half(0.6f, 0.3f);

    rigidBodies.push_back(RigidBody::createBox(Vec2(-4.0f, 1.0f), half, 1.0f));
    rigidBodies.push_back(RigidBody::createBox(Vec2(-2.0f, 1.0f), half, 1.0f));
    rigidBodies.push_back(RigidBody::createBox(Vec2(0.0f, 1.0f), half, 1.0f));
    rigidBodies.push_back(RigidBody::createCircle(Vec2(2.0f, 1.0f), 0.5f, 1.0f));

    // Static: mass 0 gives zero inverse mass and inverse inertia.
    rigidBodies.push_back(RigidBody::createBox(Vec2(4.0f, 1.0f), half, 0.0f));

    // Equal mass, radius doubled: inertia should be 4x (for Task 5-3).
    rigidBodies.push_back(RigidBody::createCircle(Vec2(-3.0f, -2.0f), 0.5f, 1.0f));
    rigidBodies.push_back(RigidBody::createCircle(Vec2(-1.0f, -2.0f), 1.0f, 1.0f));

    rigidBodies[0].rotation = 0.0f;
    rigidBodies[1].rotation = 0.5f;
    rigidBodies[2].rotation = PI / 4.0f;
    rigidBodies[3].rotation = 0.5f;
    rigidBodies[4].rotation = 0.5f;

    // Round-trip sanity check: local -> world -> local must return the input.
    const Transform t = rigidBodies[2].getTransform();
    const Vec2 p(0.3f, -0.2f);
    const Vec2 back = t.worldToLocal(t.localToWorld(p));
}

void RigidBodyScene::onRender(Renderer &renderer)
{
    for (const RigidBody &body : rigidBodies)
    {
        const Color color = body.isStatic()
                                ? Color(0.6f, 0.6f, 0.6f)
                                : Color(0.3f, 0.6f, 1.0f);
        const Transform t = body.getTransform();

        if (body.shapeType == ShapeType::Box)
        {
            renderer.drawBox(body.position, body.halfExtents, body.rotation, color);
        }
        else
        {
            renderer.drawCircle(body.position, body.radius, color);
            // Spoke so circle rotation is visible.
            renderer.drawLine(
                body.position,
                t.localToWorld(Vec2(body.radius, 0.0f)),
                color);
        }

        // Local axes: X red, Y green.
        renderer.drawLine(
            body.position,
            t.localToWorld(Vec2(AXIS_LENGTH, 0.0f)),
            Color(1.0f, 0.2f, 0.2f));
        renderer.drawLine(
            body.position,
            t.localToWorld(Vec2(0.0f, AXIS_LENGTH)),
            Color(0.2f, 1.0f, 0.2f));
    }
}
