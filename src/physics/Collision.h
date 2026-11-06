#pragma once

#include <optional>

#include "math/Vec2.h"
#include "physics/RigidBody.h"

// Contact information for one colliding pair (A, B).
// Convention: `normal` is a unit vector pointing from A toward B.
// Translating B by +normal * depth (or A by -normal * depth) separates them.
struct Manifold
{
    Vec2 normal;
    float depth = 0.0f; // always > 0
    Vec2 points[2];     // world-space contact points
    int pointCount = 0; // circles: 1, boxes: up to 2
};

// Circle vs circle on raw values (no RigidBody needed).
// Touching exactly (distance == rA + rB) is NOT a collision.
// Coincident centres: direction is undefined, so we fall back to
// normal = (1, 0), depth = rA + rB.
std::optional<Manifold> collideCircles(Vec2 centerA, float radiusA,
                                       Vec2 centerB, float radiusB);

// Convenience wrapper. Both bodies must be circles.
std::optional<Manifold> collideCircles(const RigidBody &a, const RigidBody &b);
