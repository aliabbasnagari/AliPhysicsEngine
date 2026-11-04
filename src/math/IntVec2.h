#pragma once

#include <cstddef>
#include <cstdint>

struct IntVec2
{
    int x, y;

    bool operator==(const IntVec2 &other) const
    {
        return x == other.x && y == other.y;
    }
};

// Spatial hash (large-prime multiply + xor). Unsigned arithmetic so
// negative cell coordinates wrap instead of overflowing signed ints.
struct IntVec2Hash
{
    size_t operator()(const IntVec2 &v) const
    {
        const uint32_t hx = static_cast<uint32_t>(v.x) * 73856093u;
        const uint32_t hy = static_cast<uint32_t>(v.y) * 19349663u;
        return static_cast<size_t>(hx ^ hy);
    }
};
