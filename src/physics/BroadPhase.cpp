#include "physics/BroadPhase.h"
#include "physics/AABB.h"
#include "math/IntVec2.h"

#include <algorithm>
#include <unordered_map>
#include <unordered_set>

std::vector<BodyPair> findPairsBruteForce(const std::vector<std::unique_ptr<RigidBody>> &bodies)
{
    // Compute each AABB once, not once per pair.
    std::vector<AABB> boxes;
    boxes.reserve(bodies.size());
    for (const auto &body : bodies)
    {
        boxes.push_back(body->computeAABB());
    }

    std::vector<BodyPair> pairs;
    for (size_t i = 0; i < bodies.size(); ++i)
    {
        for (size_t j = i + 1; j < bodies.size(); ++j)
        {
            if (bodies[i]->isStatic() && bodies[j]->isStatic())
            {
                continue;
            }

            if (boxes[i].overlaps(boxes[j]))
            {
                pairs.push_back({bodies[i].get(), bodies[j].get()});
            }
        }
    }
    return pairs;
}

std::vector<BodyPair> findPairsSpatialGrid(
    const std::vector<std::unique_ptr<RigidBody>> &bodies,
    float cellSize)
{
    std::unordered_map<IntVec2, std::vector<size_t>, IntVec2Hash> grid;

    for (size_t i = 0; i < bodies.size(); ++i)
    {
        const AABB box = bodies[i]->computeAABB();

        const IntVec2 minCell{
            static_cast<int>(std::floor(box.min.x / cellSize)),
            static_cast<int>(std::floor(box.min.y / cellSize))};

        const IntVec2 maxCell{
            static_cast<int>(std::floor(box.max.x / cellSize)),
            static_cast<int>(std::floor(box.max.y / cellSize))};

        for (int cx = minCell.x; cx <= maxCell.x; ++cx)
        {
            for (int cy = minCell.y; cy <= maxCell.y; ++cy)
            {
                grid[{cx, cy}].push_back(i);
            }
        }
    }

    std::vector<BodyPair> pairs;
    std::unordered_set<size_t> seen;

    for (const auto &cell : grid)
    {
        const std::vector<size_t> &indices = cell.second;

        for (size_t i = 0; i < indices.size(); ++i)
        {
            for (size_t j = i + 1; j < indices.size(); ++j)
            {
                const size_t idxA = indices[i];
                const size_t idxB = indices[j];

                if (bodies[idxA]->isStatic() && bodies[idxB]->isStatic())
                {
                    continue;
                }

                const size_t pairKey = std::min(idxA, idxB) * 1000000 + std::max(idxA, idxB);

                if (seen.insert(pairKey).second)
                {
                    pairs.push_back({bodies[idxA].get(), bodies[idxB].get()});
                }
            }
        }
    }

    return pairs;
}

void drawSpatialGrid(
    Renderer &renderer,
    const std::vector<std::unique_ptr<RigidBody>> &bodies,
    float cellSize,
    Color color)
{
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

    std::unordered_map<IntVec2, std::vector<size_t>, IntVec2Hash> grid;

    for (size_t i = 0; i < bodies.size(); ++i)
    {
        const AABB box = bodies[i]->computeAABB();

        const IntVec2 minCell{
            static_cast<int>(std::floor(box.min.x / cellSize)),
            static_cast<int>(std::floor(box.min.y / cellSize))};

        const IntVec2 maxCell{
            static_cast<int>(std::floor(box.max.x / cellSize)),
            static_cast<int>(std::floor(box.max.y / cellSize))};

        for (int cx = minCell.x; cx <= maxCell.x; ++cx)
        {
            for (int cy = minCell.y; cy <= maxCell.y; ++cy)
            {
                grid[{cx, cy}].push_back(i);
            }
        }
    }

    for (const auto &cell : grid)
    {
        const IntVec2 &cellCoord = cell.first;
        const Vec2 cellMin(
            cellCoord.x * cellSize,
            cellCoord.y * cellSize);
        const Vec2 cellMax(
            (cellCoord.x + 1) * cellSize,
            (cellCoord.y + 1) * cellSize);

        const Vec2 center = (cellMin + cellMax) * 0.5f;
        const Vec2 halfExtents = (cellMax - cellMin) * 0.5f;

        renderer.drawBox(center, halfExtents, 0.0f, color);
    }
}