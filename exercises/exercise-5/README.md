# Exercise 5 — Rigid Body Fundamentals

**Tier:** 2 — 2D Rigid Bodies
**Prerequisites:** Exercise 3 (integrators), Exercise 1 (math)

This is where the engine stops being "particles" and starts being a real
physics engine: bodies that have *orientation* and can *rotate*, not just
translate. Everything from here through Exercise 10 builds the 2D rigid
body pipeline: this module is the data model and integration; Exercises 6-9
are collision and resolution; Exercise 10 is joints.

## What you'll build

A `RigidBody` with mass/inertia, torque and angular velocity, and a
`PhysicsWorld` that steps a collection of them.

## Files you'll touch

- `src/physics/RigidBody.h` / `.cpp`
- `src/physics/PhysicsWorld.h` / `.cpp`

## Tasks

1. [Task 5-1: RigidBody Data](task5-1.md)
2. [Task 5-2: Moment of Inertia](task5-2.md)
3. [Task 5-3: Force, Torque & Angular Integration](task5-3.md)
4. [Task 5-4: PhysicsWorld Step](task5-4.md)
