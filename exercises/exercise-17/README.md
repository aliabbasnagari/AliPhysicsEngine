# Exercise 17 — Multithreading & Determinism

**Tier:** 6 — God Tier: Performance & Architecture
**Prerequisites:** Exercise 16, Exercise 9 (sleeping/islands)

Two related, often-conflated-but-actually-different topics: making the
physics step use multiple CPU cores (performance), and making it produce
*bit-identical* results given the same inputs, run after run, possibly on
different machines (determinism — needed for replays, lockstep networking,
and reproducible bug reports).

## What you'll build

Parallel broad-phase pair generation, island-based parallel solving, a
deterministic fixed-timestep + deterministic-math discipline, and a
lockstep record/replay harness to prove it.

## Files you'll touch

- `src/physics/PhysicsWorld.h` / `.cpp`
- New `src/core/JobSystem.h` / `.cpp`, `src/core/Replay.h` / `.cpp`

## Tasks

1. [Task 17-1: Parallel Broad-Phase](task17-1.md)
2. [Task 17-2: Island-Based Parallel Solving](task17-2.md)
3. [Task 17-3: Deterministic Simulation](task17-3.md)
4. [Task 17-4: Lockstep Replay Harness](task17-4.md)
