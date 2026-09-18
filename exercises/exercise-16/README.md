# Exercise 16 — Data-Oriented Performance

**Tier:** 6 — God Tier: Performance & Architecture
**Prerequisites:** Exercise 9, Exercise 12

Everything up to here has prioritized correctness over speed — reasonably
so; you can't optimize what doesn't work yet. This module is about making
the (now-correct) engine fast, using measurement-driven, data-oriented
techniques: profiling, cache-friendly memory layout, SIMD, and custom
allocation.

## What you'll build

An in-engine profiler overlay, a structure-of-arrays refactor, a
SIMD-accelerated hot path, and a custom allocator for per-frame contact
data.

## Files you'll touch

- `src/physics/PhysicsWorld.h` / `.cpp`
- `src/physics/RigidBody.h` / `.cpp`
- New `src/core/Profiler.h` / `.cpp`, `src/core/Allocator.h` / `.cpp`

## Tasks

1. [Task 16-1: Profile Your Physics Step](task16-1.md)
2. [Task 16-2: Structure-of-Arrays Refactor](task16-2.md)
3. [Task 16-3: SIMD a Hot Path](task16-3.md)
4. [Task 16-4: Custom Allocator / Object Pool](task16-4.md)
