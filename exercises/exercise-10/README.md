# Exercise 10 — Joints & Constraints

**Tier:** 2 — 2D Rigid Bodies
**Prerequisites:** Exercise 9

Joints connect two rigid bodies with a constraint that isn't collision —
"stay this far apart," "rotate around this shared point," "slide only
along this axis." Mechanically, they're solved the same way as contacts
(Exercise 8-9): compute a Jacobian/effective mass, solve for an impulse
that satisfies a velocity constraint, apply, iterate.

## What you'll build

Distance, revolute (pin), prismatic (slider), and spring/motor joints.

## Files you'll touch

- New `src/physics/Joint.h` / `.cpp`
- `src/physics/PhysicsWorld.h` / `.cpp`

## Tasks

1. [Task 10-1: Distance Joint](task10-1.md)
2. [Task 10-2: Revolute Joint](task10-2.md)
3. [Task 10-3: Prismatic Joint](task10-3.md)
4. [Task 10-4: Spring/Motor Joint](task10-4.md)
