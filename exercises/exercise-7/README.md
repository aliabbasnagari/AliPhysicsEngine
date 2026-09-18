# Exercise 7 — Narrow-Phase Collision Detection

**Tier:** 2 — 2D Rigid Bodies
**Prerequisites:** Exercise 6

Broad phase gave you candidate pairs. Narrow phase determines, exactly,
whether two specific shapes overlap and — critically for Exercise 8 — the
contact point(s), normal, and penetration depth needed to resolve it.

## What you'll build

Exact intersection tests for circle-circle, circle-polygon/AABB, and
general convex polygon vs polygon via the Separating Axis Theorem, plus a
contact manifold structure.

## Files you'll touch

- New `src/physics/Collision.h` / `.cpp` (or similar — this deserves its own file)
- `src/physics/PhysicsWorld.h` / `.cpp`

## Tasks

1. [Task 7-1: Circle vs Circle](task7-1.md)
2. [Task 7-2: Circle vs AABB/OBB](task7-2.md)
3. [Task 7-3: Polygon SAT](task7-3.md)
4. [Task 7-4: Contact Manifold Generation](task7-4.md)
