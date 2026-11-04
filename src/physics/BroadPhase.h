#pragma once

#include <cstddef>
#include <memory>
#include <set>
#include <utility>
#include <vector>

#include "math/IntVec2.h"
#include "physics/AABB.h"
#include "physics/RigidBody.h"

struct BodyPair
{
    const RigidBody *a;
    const RigidBody *b;
};

// Work counters so broad phase algorithms can be compared.
struct BroadPhaseStats
{
    // Number of AABB-vs-AABB overlap tests performed (the "pair-vs-pair
    // comparisons"). Brute force: one per non-static-static pair.
    // Grid: one per distinct pair that shares at least one cell.
    size_t overlapTests = 0;

    // Grid only: body-pairs enumerated inside cells, before dedup.
    // The same pair is counted once per shared cell.
    size_t cellPairVisits = 0;

    // Grid only: total (body, cell) insertions.
    size_t cellInsertions = 0;
};

// Both functions return exactly the pairs whose AABBs overlap, skipping
// static-static pairs. `stats` is optional.
std::vector<BodyPair> findPairsBruteForce(
    const std::vector<std::unique_ptr<RigidBody>> &bodies,
    BroadPhaseStats *stats = nullptr);

// Uniform spatial grid. Result is independent of cellSize (only the
// amount of work changes). cellSize must be > 0.
std::vector<BodyPair> findPairsSpatialGrid(
    const std::vector<std::unique_ptr<RigidBody>> &bodies,
    float cellSize,
    BroadPhaseStats *stats = nullptr);

// Inclusive range of grid cells an AABB touches.
struct CellRange
{
    IntVec2 min;
    IntVec2 max;
};

CellRange computeCellRange(const AABB &box, float cellSize);

// Every grid cell occupied by at least one body (for debug drawing).
std::vector<IntVec2> findOccupiedCells(
    const std::vector<std::unique_ptr<RigidBody>> &bodies,
    float cellSize);

// Order-independent pair set: (A,B) and (B,A) compare equal.
using PairSet = std::set<std::pair<const RigidBody *, const RigidBody *>>;

PairSet makePairSet(const std::vector<BodyPair> &pairs);
