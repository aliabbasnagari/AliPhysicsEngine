# Exercise 8 — Collision Resolution

**Tier:** 2 — 2D Rigid Bodies
**Prerequisites:** Exercise 7

You can detect contacts now. This module makes bodies actually *respond* to
them: bouncing, stopping, and sliding realistically, using **impulses**
(instantaneous velocity changes) rather than forces.

## What you'll build

Normal impulse resolution with restitution, positional penetration
correction, Coulomb friction, and multi-point/angular impulse handling.

## Files you'll touch

- New `src/physics/Solver.h` / `.cpp` (or extend `PhysicsWorld`, your call)
- `src/physics/PhysicsWorld.h` / `.cpp`

## Tasks

1. [Task 8-1: Normal Impulse & Restitution](task8-1.md)
2. [Task 8-2: Positional Correction](task8-2.md)
3. [Task 8-3: Friction](task8-3.md)
4. [Task 8-4: Angular Impulse from Contacts](task8-4.md)
