#include "scenes/RigidBodyScene.h"

#include "graphics/Renderer.h"

namespace
{
    constexpr float AXIS_LENGTH = 0.6f;
    constexpr float ARROW_SCALE = 2.0f; // world units per newton
    constexpr float RESET_SECONDS = 6.0f;
}

void RigidBodyScene::addTest(
    const RigidBody &body, Vec2 force, Vec2 localPoint, float torque)
{
    rigidBodies.push_back(body);
    pushes.push_back(Push{rigidBodies.size() - 1, force, localPoint, torque});
}

void RigidBodyScene::onEnter()
{
    // Re-entrant: this object outlives each visit to the scene.
    rigidBodies.clear();
    pushes.clear();
    elapsed = 0.0f;

    // No gravity here, so the effect of each load is easy to see.
    const Vec2 half(0.6f, 0.3f);
    const Vec2 up(0.0f, 0.3f);
    const float y = -3.0f;

    // 1. Force through the centre of mass: translates, never spins.
    addTest(RigidBody::createBox(Vec2(-7.5f, y), half, 1.0f), up, Vec2(0.0f, 0.0f), 0.0f);

    // 2. Same force at the right edge: translates and spins.
    addTest(RigidBody::createBox(Vec2(-5.0f, y), half, 1.0f), up, Vec2(half.x, 0.0f), 0.0f);

    // 3. Mirrored point: spin direction reverses.
    addTest(RigidBody::createBox(Vec2(-2.5f, y), half, 1.0f), up, Vec2(-half.x, 0.0f), 0.0f);

    // 4. Torque only: spins in place.
    addTest(RigidBody::createBox(Vec2(0.0f, y), half, 1.0f), Vec2(0.0f, 0.0f), Vec2(0.0f, 0.0f), 0.3f);

    // 5. Static body under force and torque: must not move or rotate.
    RigidBody staticBox = RigidBody::createBox(Vec2(2.5f, y), half, 0.0f);
    staticBox.rotation = 0.5f;
    addTest(staticBox, up, Vec2(half.x, 0.0f), 0.3f);

    // 6. Equal mass and torque, radius doubled: the big one spins up slower.
    addTest(RigidBody::createCircle(Vec2(5.0f, y), 0.5f, 1.0f), Vec2(0.0f, 0.0f), Vec2(0.0f, 0.0f), 0.3f);
    addTest(RigidBody::createCircle(Vec2(7.5f, y), 1.0f, 1.0f), Vec2(0.0f, 0.0f), Vec2(0.0f, 0.0f), 0.3f);
}

void RigidBodyScene::onUpdate(float fixedDt)
{
    for (const Push &push : pushes)
    {
        RigidBody &body = rigidBodies[push.body];

        if (push.force.lengthSquared() > 0.0f)
        {
            // Application point must be in world space.
            body.applyForceAtPoint(
                push.force,
                body.getTransform().localToWorld(push.localPoint));
        }
        if (push.torque != 0.0f)
        {
            body.applyTorque(push.torque);
        }
    }

    for (RigidBody &body : rigidBodies)
    {
        body.integrate(fixedDt);
    }

    // Bodies drift off screen with no gravity or walls; restart the demo.
    elapsed += fixedDt;
    if (elapsed > RESET_SECONDS)
    {
        onEnter();
    }
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

    // Force arrows from each application point.
    for (const Push &push : pushes)
    {
        if (push.force.lengthSquared() <= 0.0f)
        {
            continue;
        }
        const RigidBody &body = rigidBodies[push.body];
        const Vec2 from = body.getTransform().localToWorld(push.localPoint);
        renderer.drawLine(from, from + push.force * ARROW_SCALE, Color(1.0f, 0.9f, 0.2f));
    }
}
