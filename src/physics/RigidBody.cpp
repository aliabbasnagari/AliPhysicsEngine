#include "physics/RigidBody.h"

#include <algorithm>
#include <cmath>

Vec2 Transform::localToWorld(Vec2 local) const
{
    const float c = std::cos(rotation);
    const float s = std::sin(rotation);
    return Vec2(
        local.x * c - local.y * s + position.x,
        local.x * s + local.y * c + position.y);
}

Vec2 Transform::worldToLocal(Vec2 world) const
{
    const float c = std::cos(rotation);
    const float s = std::sin(rotation);
    const Vec2 d = world - position;
    // Rotate by -rotation.
    return Vec2(
        d.x * c + d.y * s,
        -d.x * s + d.y * c);
}

RigidBody::RigidBody(
    Vec2 position, ShapeType shapeType, float mass, float inertia)
    : position(position),
      mass(mass),
      inverseMass(mass > 0.0f ? 1.0f / mass : 0.0f),
      inertia(inertia),
      inverseInertia(inertia > 0.0f ? 1.0f / inertia : 0.0f),
      shapeType(shapeType)
{
}

AABB RigidBody::computeAABB() const
{
    if (shapeType == ShapeType::Circle)
    {
        const Vec2 extent(radius, radius);
        return AABB{position - extent, position + extent};
    }

    // Box: a rotated box's AABB is built from its rotated corners, not
    // from the unrotated half-extents.
    const Transform t = getTransform();
    const Vec2 corners[4] = {
        t.localToWorld(Vec2(-halfExtents.x, -halfExtents.y)),
        t.localToWorld(Vec2(halfExtents.x, -halfExtents.y)),
        t.localToWorld(Vec2(halfExtents.x, halfExtents.y)),
        t.localToWorld(Vec2(-halfExtents.x, halfExtents.y)),
    };

    // Seed from the first corner, not zero, so bodies far from the
    // origin are handled correctly.
    AABB box{corners[0], corners[0]};
    for (int i = 1; i < 4; ++i)
    {
        box.min.x = std::min(box.min.x, corners[i].x);
        box.min.y = std::min(box.min.y, corners[i].y);
        box.max.x = std::max(box.max.x, corners[i].x);
        box.max.y = std::max(box.max.y, corners[i].y);
    }
    return box;
}

RigidBody RigidBody::createCircle(
    Vec2 position, float radius, float mass)
{
    return createCircle(position, radius, mass, 0.5f * mass * radius * radius);
}

RigidBody RigidBody::createCircle(
    Vec2 position, float radius, float mass, float inertia)
{
    // Static (mass <= 0) bodies get zero inertia automatically.
    RigidBody body(position, ShapeType::Circle, mass, mass > 0.0f ? inertia : 0.0f);
    body.radius = radius;
    return body;
}

RigidBody RigidBody::createBox(
    Vec2 position, Vec2 halfExtents, float mass)
{
    // The formula uses full width/height, not half-extents.
    const float w = 2.0f * halfExtents.x;
    const float h = 2.0f * halfExtents.y;
    return createBox(position, halfExtents, mass, (1.0f / 12.0f) * mass * (w * w + h * h));
}

RigidBody RigidBody::createBox(
    Vec2 position, Vec2 halfExtents, float mass, float inertia)
{
    RigidBody body(position, ShapeType::Box, mass, mass > 0.0f ? inertia : 0.0f);
    body.halfExtents = halfExtents;
    return body;
}

void RigidBody::applyForce(Vec2 force)
{
    forceAccumulator += force;
}
void RigidBody::applyForceAtPoint(Vec2 force, Vec2 worldPoint)
{
    // Apply the force to the linear accumulator.
    applyForce(force);

    // Compute the torque: r x F, where r is the vector from the center of mass
    // to the point of application.
    const Vec2 r = worldPoint - position;
    const float torque = r.cross(force);
    applyTorque(torque);
}
void RigidBody::applyTorque(float torque)
{

    torqueAccumulator += torque;
}
void RigidBody::clearForces()
{
    forceAccumulator = Vec2(0.0f, 0.0f);
    torqueAccumulator = 0.0f;
}
void RigidBody::integrate(float dt)
{

    if (isStatic())
    {
        // Discard anything applied so it can't pile up.
        clearForces();
        return;
    }

    // Update linear velocity and position.
    const Vec2 acceleration = forceAccumulator * inverseMass;
    linearVelocity += acceleration * dt;
    position += linearVelocity * dt;

    // Update angular velocity and rotation.
    const float angularAcceleration = torqueAccumulator * inverseInertia;
    angularVelocity += angularAcceleration * dt;
    rotation += angularVelocity * dt;
    constexpr float PI = 3.14159265358979f;
    if (rotation > PI || rotation < -PI)
    {
        rotation = std::remainder(rotation, 2.0f * PI);
    }

    // Clear the accumulators for the next step.
    clearForces();
}