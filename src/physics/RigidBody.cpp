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
    Vec2 position, float radius, float mass, float inertia)
{
    RigidBody body(position, ShapeType::Circle, mass, inertia);
    body.radius = radius;
    return body;
}

RigidBody RigidBody::createBox(
    Vec2 position, Vec2 halfExtents, float mass, float inertia)
{
    RigidBody body(position, ShapeType::Box, mass, inertia);
    body.halfExtents = halfExtents;
    return body;
}
