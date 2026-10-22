#pragma once

#include <unordered_map>

struct IntVec2
{
    int x, y;

    bool operator==(const IntVec2 &other) const
    {
        return x == other.x && y == other.y;
    }
};

struct IntVec2Hash
{
    size_t operator()(const IntVec2 &v) const
    {
        return std::hash<int>()(v.x) ^ (std::hash<int>()(v.y) << 1);
    }
};