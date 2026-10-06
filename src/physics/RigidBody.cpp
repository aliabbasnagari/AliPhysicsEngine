#include "physics/RigidBody.h"

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

RigidBody RigidBody::createCircle(
    Vec2 position, float radius, float mass)
{
    // Static (mass <= 0) bodies get zero inertia automatically.
    const float inertia = mass > 0.0f ? 0.5f * mass * radius * radius : 0.0f;
    RigidBody body(position, ShapeType::Circle, mass, inertia);
    body.radius = radius;
    return body;
}

RigidBody RigidBody::createBox(
    Vec2 position, Vec2 halfExtents, float mass)
{
    // The formula uses full width/height, not half-extents.
    const float w = 2.0f * halfExtents.x;
    const float h = 2.0f * halfExtents.y;
    const float inertia = mass > 0.0f ? (1.0f / 12.0f) * mass * (w * w + h * h) : 0.0f;
    RigidBody body(position, ShapeType::Box, mass, inertia);
    body.halfExtents = halfExtents;
    return body;
}
