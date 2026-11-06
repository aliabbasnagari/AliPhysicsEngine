#include "physics/Collision.h"

#include <cassert>
#include <cmath>

namespace
{
    constexpr float kCoincidentEpsilonSq = 1e-12f;
}

std::optional<Manifold> collideCircles(Vec2 centerA, float radiusA,
                                       Vec2 centerB, float radiusB)
{
    const Vec2 delta = centerB - centerA;
    const float radiusSum = radiusA + radiusB;
    const float distSq = delta.lengthSquared();

    // Early-out without a sqrt.
    if (distSq >= radiusSum * radiusSum)
        return std::nullopt;

    Manifold m;
    m.pointCount = 1;

    if (distSq <= kCoincidentEpsilonSq)
    {
        m.normal = Vec2(1.0f, 0.0f);
        m.depth = radiusSum;
        m.points[0] = centerA;
        return m;
    }

    const float dist = std::sqrt(distSq);
    m.normal = delta / dist;
    m.depth = radiusSum - dist;

    const Vec2 surfaceA = centerA + m.normal * radiusA;
    const Vec2 surfaceB = centerB - m.normal * radiusB;
    m.points[0] = (surfaceA + surfaceB) * 0.5f;
    return m;
}

std::optional<Manifold> collideCircles(const RigidBody &a, const RigidBody &b)
{
    assert(a.shapeType == ShapeType::Circle && b.shapeType == ShapeType::Circle);
    return collideCircles(a.position, a.radius, b.position, b.radius);
}
