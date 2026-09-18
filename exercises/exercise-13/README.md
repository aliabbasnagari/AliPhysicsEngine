# Exercise 13 — 3D Math & Rigid Bodies

**Tier:** 4 — Into 3D
**Prerequisites:** Exercise 5, Exercise 1 (Vec3/Quaternion)

Everything from Exercise 5 through 12 was 2D, where orientation is a single
angle. 3D orientation needs a quaternion (Task 1-4) and inertia becomes a
**tensor** (a 3x3 matrix), not a scalar — because a 3D object can resist
rotation differently around different axes. This module rebuilds the
Exercise 5 rigid body pipeline for 3D.

## What you'll build

3D inertia tensors, quaternion-based orientation integration, and a 3D
`RigidBody`/`PhysicsWorld` step.

## Files you'll touch

- `src/physics/RigidBody.h` / `.cpp` (extend or add a 3D variant — your call on how much to share with the 2D version)
- `src/physics/PhysicsWorld.h` / `.cpp`

## Tasks

1. [Task 13-1: 3D Inertia Tensors](task13-1.md)
2. [Task 13-2: Quaternion-Based Orientation Integration](task13-2.md)
3. [Task 13-3: 3D RigidBody & PhysicsWorld Step](task13-3.md)
