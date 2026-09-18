# Exercise 9 — Stable Contacts & Solver Quality

**Tier:** 2 — 2D Rigid Bodies
**Prerequisites:** Exercise 8

Exercise 8 gives you *correct* collision response for a single contact
resolved once. Real engines resolve many simultaneous contacts, iteratively,
and need to do it every frame without jittering or exploding. This module
is what turns "physics that mostly works" into "physics that looks like a
real engine" — stable stacks, quiet resting bodies, sleeping when idle.

## What you'll build

Persistent contact manifolds, warm starting, a proper sequential-impulse
solver loop, and sleeping bodies.

## Files you'll touch

- `src/physics/Solver.h` / `.cpp`
- `src/physics/PhysicsWorld.h` / `.cpp`

## Tasks

1. [Task 9-1: Persistent Contact Manifolds](task9-1.md)
2. [Task 9-2: Warm Starting](task9-2.md)
3. [Task 9-3: Sequential Impulse Solver Loop](task9-3.md)
4. [Task 9-4: Sleeping Bodies](task9-4.md)
