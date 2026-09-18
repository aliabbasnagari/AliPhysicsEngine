# Exercise 15 — Position-Based & Extended Position-Based Dynamics

**Tier:** 5 — Soft Bodies
**Prerequisites:** Exercise 4 (constraint-based particles), Exercise 9 (iterative solving)

Exercise 4 already used position-based constraint solving informally for
rope/cloth. This module formalizes it (Position-Based Dynamics, generalized
to arbitrary constraints, not just distance) and then fixes PBD's biggest
weakness — **timestep/iteration-count-dependent stiffness** — with XPBD, a
technique from actual modern research (2016) used in production engines.

## What you'll build

A generalized PBD constraint-projection solver, XPBD (compliance-based,
substeps), a proper cloth sim reusing it, and a volume-preserving soft body
blob.

## Files you'll touch

- New `src/physics/PBDSolver.h` / `.cpp`
- `src/sandbox/Sandbox.h` / `.cpp`

## Tasks

1. [Task 15-1: Generalized PBD Solver](task15-1.md)
2. [Task 15-2: XPBD](task15-2.md)
3. [Task 15-3: Cloth via PBD](task15-3.md)
4. [Task 15-4: Soft-Body Blob](task15-4.md)
