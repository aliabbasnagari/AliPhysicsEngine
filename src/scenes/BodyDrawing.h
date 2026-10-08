#pragma once

#include "graphics/Renderer.h"
#include "physics/RigidBody.h"

// Shared by the rigid body scenes. Draws the shape plus its local axes
// (X red, Y green) so rotation is always visible.
inline void drawBody(Renderer &renderer, const RigidBody &body)
{
    constexpr float axisLength = 0.6f;

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
            body.position, t.localToWorld(Vec2(body.radius, 0.0f)), color);
    }

    renderer.drawLine(
        body.position, t.localToWorld(Vec2(axisLength, 0.0f)),
        Color(1.0f, 0.2f, 0.2f));
    renderer.drawLine(
        body.position, t.localToWorld(Vec2(0.0f, axisLength)),
        Color(0.2f, 1.0f, 0.2f));
}
