#pragma once

#include "math/Vec2.h"

struct AABB
{
    Vec2 min;
    Vec2 max;

    // Touching edges count as overlapping: broad phase must never miss a
    // real pair, and the narrow phase rejects false positives.
    bool overlaps(const AABB &other) const
    {
        return !(max.x < other.min.x || min.x > other.max.x ||
                 max.y < other.min.y || min.y > other.max.y);
    }

    Vec2 center() const
    {
        return (min + max) * 0.5f;
    }

    Vec2 size() const
    {
        return max - min;
    }
};
