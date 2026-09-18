# Exercise 3 — Particles & Integrators

**Tier:** 1 — Particle Physics
**Prerequisites:** Exercise 1, Exercise 2

A particle is the simplest physical object: a point mass with no rotation.
This module is about **integration** — how you turn forces into motion over
time — which is the single most important numerical-stability concept in
the whole engine. Get this intuition right here, on the simplest possible
object, before it's tangled up with rotation and collision.

## What you'll build

A `Particle` type, several integration schemes you can compare side by
side, and a generic force accumulator pattern.

## Files you'll touch

- `src/physics/RigidBody.h` / `.cpp` (start here with a minimal particle;
  you'll extend this same type with orientation/inertia in Exercise 5)
- `src/physics/PhysicsWorld.h` / `.cpp`
- `src/sandbox/Sandbox.h` / `.cpp`

## Tasks

1. [Task 3-1: Particle Under Gravity (Explicit Euler)](task3-1.md)
2. [Task 3-2: Semi-Implicit Euler](task3-2.md)
3. [Task 3-3: Verlet Integration](task3-3.md)
4. [Task 3-4: Drag / Damping](task3-4.md)
5. [Task 3-5: Force Accumulator Pattern](task3-5.md)
