# Exercise 11 — Continuous Collision Detection & Raycasting

**Tier:** 3 — Scaling Up 2D
**Prerequisites:** Exercise 7, Exercise 9

Everything so far is **discrete** collision detection: check for overlap at
each fixed timestep. Fast-moving small objects can pass straight through
thin geometry between two steps without ever overlapping at either sampled
instant ("tunneling"). This module covers raycasting (broadly useful on its
own — mouse picking, line-of-sight, bullets) and continuous collision
detection (CCD) to catch fast movers.

## What you'll build

Ray-vs-shape tests, a deliberate tunneling demonstration, and a swept-shape
CCD pass for fast-moving bodies.

## Files you'll touch

- New `src/physics/Raycast.h` / `.cpp`
- `src/physics/PhysicsWorld.h` / `.cpp`

## Tasks

1. [Task 11-1: Ray vs Shape](task11-1.md)
2. [Task 11-2: Demonstrate Tunneling](task11-2.md)
3. [Task 11-3: Conservative Advancement CCD](task11-3.md)
