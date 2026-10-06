#pragma once

#include "math/Vec2.h"

// Conventions
// - Rotation: radians, positive = counter-clockwise on screen
//   (+Y up, matching Renderer).
// - Static: inverseMass == 0 && inverseInertia == 0. A static body
//   never responds to forces or torques. Use isStatic() instead of
//   re-checking the inverses at call sites.
// - Units: world units, kg, radians, radians/sec.

enum class ShapeType
{
    Circle,
    Box
};

// Position + orientation. Maps body-local points into world space.
struct Transform
{
    Vec2 position;
    float rotation = 0.0f;

    Vec2 localToWorld(Vec2 local) const;
    Vec2 worldToLocal(Vec2 world) const;
};

class RigidBody
{
public:
    Vec2 position;
    float rotation = 0.0f;
    Vec2 linearVelocity;
    float angularVelocity = 0.0f;

    float mass = 0.0f;
    float inverseMass = 0.0f;
    float inertia = 0.0f;
    float inverseInertia = 0.0f;

    ShapeType shapeType = ShapeType::Circle;
    float radius = 0.0f; // Circle only
    Vec2 halfExtents;    // Box only

    Vec2 forceAccumulator;
    float torqueAccumulator = 0.0f;

    // Inertia is computed from mass and shape (uniform density, about the
    // centre of mass). mass <= 0 makes the body static: zero inverse mass
    // and zero inverse inertia.
    static RigidBody createCircle(
        Vec2 position, float radius, float mass);
    static RigidBody createBox(
        Vec2 position, Vec2 halfExtents, float mass);

    bool isStatic() const
    {
        return inverseMass == 0.0f && inverseInertia == 0.0f;
    }

    Transform getTransform() const { return Transform{position, rotation}; }

    void applyForce(Vec2 force);
    void applyForceAtPoint(Vec2 force, Vec2 worldPoint);
    void applyTorque(float torque);
    void clearForces();
    void integrate(float dt);

private:
    RigidBody(Vec2 position, ShapeType shapeType, float mass, float inertia);
};
