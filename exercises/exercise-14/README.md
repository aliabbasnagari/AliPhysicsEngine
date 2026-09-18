# Exercise 14 — 3D Collision Detection

**Tier:** 4 — Into 3D
**Prerequisites:** Exercise 13, Exercise 7 (SAT intuition), Exercise 8 (impulse resolution)

3D SAT (testing face normals and edge-edge cross products) works but is
fiddly to get right for general convex shapes. **GJK** (Gilbert-Johnson-Keerthi)
is the standard general-purpose convex-vs-convex distance/intersection
test, and **EPA** (Expanding Polytope Algorithm) extends it to give
penetration depth and normal once GJK finds an overlap. This is widely
considered one of the hardest modules in the whole curriculum — budget
real time for it.

## What you'll build

Sphere-based 3D tests as a warm-up, then a general GJK intersection test,
EPA for penetration info, and 3D contact generation feeding your existing
(Exercise 8) impulse solver.

## Files you'll touch

- New `src/physics/GJK.h` / `.cpp`, `src/physics/EPA.h` / `.cpp`
- `src/physics/Collision.h` / `.cpp`

## Tasks

1. [Task 14-1: Sphere vs Sphere, Sphere vs AABB](task14-1.md)
2. [Task 14-2: GJK](task14-2.md)
3. [Task 14-3: EPA](task14-3.md)
4. [Task 14-4: 3D Contact Generation & Resolution](task14-4.md)
