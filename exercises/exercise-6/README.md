# Exercise 6 — Broad-Phase Collision Detection

**Tier:** 2 — 2D Rigid Bodies
**Prerequisites:** Exercise 5

Testing every body against every other body with exact shape math is
wasteful. **Broad phase** cheaply narrows down *which pairs* might be
colliding (using cheap bounding volumes), so the expensive exact test
(Exercise 7) only runs on plausible candidates.

## What you'll build

AABB computation, a brute-force broad phase, and a spatial grid broad
phase.

## Files you'll touch

- `src/physics/PhysicsWorld.h` / `.cpp`
- Possibly a new `src/physics/BroadPhase.h` / `.cpp` — your call on whether it earns its own file yet

## Tasks

1. [Task 6-1: AABB Computation](task6-1.md)
2. [Task 6-2: Brute-Force Broad Phase](task6-2.md)
3. [Task 6-3: Spatial Grid Broad Phase](task6-3.md)
