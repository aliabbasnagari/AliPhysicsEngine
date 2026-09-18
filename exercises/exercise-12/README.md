# Exercise 12 — Spatial Partitioning at Scale

**Tier:** 3 — Scaling Up 2D
**Prerequisites:** Exercise 6

Exercise 6's spatial grid works well for evenly-distributed bodies at a
tuned cell size, but degrades for uneven distributions (everything clustered
in one area, or wildly varying body sizes) and needs manual tuning. This
module covers two structures that don't have that weakness: sweep and
prune, and a dynamic bounding volume hierarchy (BVH) — the latter being
what most production 2D/3D engines actually use.

## What you'll build

A sweep-and-prune broad phase, a dynamic AABB tree, and a benchmark
comparing all four broad-phase approaches you'll have built by the end of
this module.

## Files you'll touch

- `src/physics/BroadPhase.h` / `.cpp` (from Exercise 6, or wherever you put it)

## Tasks

1. [Task 12-1: Sweep and Prune](task12-1.md)
2. [Task 12-2: Dynamic AABB Tree (BVH)](task12-2.md)
3. [Task 12-3: Benchmark Comparison](task12-3.md)
