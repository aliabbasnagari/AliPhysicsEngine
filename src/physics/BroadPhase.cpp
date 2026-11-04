#include "physics/BroadPhase.h"

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <unordered_map>
#include <unordered_set>

namespace
{
    using CellMap = std::unordered_map<IntVec2, std::vector<uint32_t>, IntVec2Hash>;

    std::vector<AABB> computeAllAABBs(
        const std::vector<std::unique_ptr<RigidBody>> &bodies)
    {
        // Compute each AABB once, not once per pair.
        std::vector<AABB> boxes;
        boxes.reserve(bodies.size());
        for (const auto &body : bodies)
        {
            boxes.push_back(body->computeAABB());
        }
        return boxes;
    }

    // Float -> int cell coordinate. Clamped so a body that flies far away
    // (or a NaN/inf position) can't trigger undefined behaviour in the cast.
    int toCell(float coord, float cellSize)
    {
        constexpr float limit = 1.0e8f;
        const float cell = std::floor(coord / cellSize);
        if (!(cell > -limit)) // also catches NaN
        {
            return static_cast<int>(-limit);
        }
        if (cell > limit)
        {
            return static_cast<int>(limit);
        }
        return static_cast<int>(cell);
    }

    CellMap buildGrid(const std::vector<AABB> &boxes, float cellSize, BroadPhaseStats *stats)
    {
        CellMap grid;
        for (size_t i = 0; i < boxes.size(); ++i)
        {
            const CellRange range = computeCellRange(boxes[i], cellSize);
            for (int cx = range.min.x; cx <= range.max.x; ++cx)
            {
                for (int cy = range.min.y; cy <= range.max.y; ++cy)
                {
                    grid[{cx, cy}].push_back(static_cast<uint32_t>(i));
                    if (stats)
                    {
                        ++stats->cellInsertions;
                    }
                }
            }
        }
        return grid;
    }
}

CellRange computeCellRange(const AABB &box, float cellSize)
{
    return {
        {toCell(box.min.x, cellSize), toCell(box.min.y, cellSize)},
        {toCell(box.max.x, cellSize), toCell(box.max.y, cellSize)}};
}

std::vector<BodyPair> findPairsBruteForce(
    const std::vector<std::unique_ptr<RigidBody>> &bodies,
    BroadPhaseStats *stats)
{
    const std::vector<AABB> boxes = computeAllAABBs(bodies);

    std::vector<BodyPair> pairs;
    for (size_t i = 0; i < bodies.size(); ++i)
    {
        for (size_t j = i + 1; j < bodies.size(); ++j)
        {
            if (bodies[i]->isStatic() && bodies[j]->isStatic())
            {
                continue;
            }

            if (stats)
            {
                ++stats->overlapTests;
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
    float cellSize,
    BroadPhaseStats *stats)
{
    if (!(cellSize > 0.0f))
    {
        return {};
    }

    // 1. Insert every body into every cell its AABB overlaps.
    const std::vector<AABB> boxes = computeAllAABBs(bodies);
    const CellMap grid = buildGrid(boxes, cellSize, stats);

    // 2. Candidate pairs come only from bodies sharing a cell. Two bodies
    //    can share several cells, so each pair is deduped by index key
    //    (independent of cell size) before it is tested.
    std::unordered_set<uint64_t> tested;
    std::vector<std::pair<uint32_t, uint32_t>> hits;

    for (const auto &cell : grid)
    {
        const std::vector<uint32_t> &indices = cell.second;

        for (size_t i = 0; i < indices.size(); ++i)
        {
            for (size_t j = i + 1; j < indices.size(); ++j)
            {
                if (stats)
                {
                    ++stats->cellPairVisits;
                }

                // Indices are inserted in ascending order, so a < b.
                const uint32_t a = indices[i];
                const uint32_t b = indices[j];

                if (bodies[a]->isStatic() && bodies[b]->isStatic())
                {
                    continue;
                }

                const uint64_t key = (static_cast<uint64_t>(a) << 32) | b;
                if (!tested.insert(key).second)
                {
                    continue; // already handled via another shared cell
                }

                if (stats)
                {
                    ++stats->overlapTests;
                }

                // Sharing a cell is not overlap: AABBs can sit in the same
                // cell without touching. Filter so the result matches
                // brute force exactly.
                if (boxes[a].overlaps(boxes[b]))
                {
                    hits.emplace_back(a, b);
                }
            }
        }
    }

    // unordered_map iteration order is arbitrary; sort so output order is
    // deterministic and identical to brute force.
    std::sort(hits.begin(), hits.end());

    std::vector<BodyPair> pairs;
    pairs.reserve(hits.size());
    for (const auto &hit : hits)
    {
        pairs.push_back({bodies[hit.first].get(), bodies[hit.second].get()});
    }
    return pairs;
}

std::vector<IntVec2> findOccupiedCells(
    const std::vector<std::unique_ptr<RigidBody>> &bodies,
    float cellSize)
{
    std::vector<IntVec2> cells;
    if (!(cellSize > 0.0f))
    {
        return cells;
    }

    const CellMap grid = buildGrid(computeAllAABBs(bodies), cellSize, nullptr);
    cells.reserve(grid.size());
    for (const auto &cell : grid)
    {
        cells.push_back(cell.first);
    }
    return cells;
}

PairSet makePairSet(const std::vector<BodyPair> &pairs)
{
    PairSet set;
    for (const BodyPair &pair : pairs)
    {
        set.insert(std::minmax<const RigidBody *>(pair.a, pair.b));
    }
    return set;
}
