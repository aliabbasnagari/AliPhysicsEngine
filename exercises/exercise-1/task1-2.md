# Task 1-2: Vec3

**Module:** Exercise 1 — Math Foundations
**Goal:** A working 3D vector type, needed for 3D rigid bodies (Exercise 13+) and as the base type for angular velocity/torque even in some 2D formulations.

## Concept
Same idea as `Vec2` plus a third axis. The new operation is the **cross
product**: `a × b` gives a vector perpendicular to both `a` and `b`, with
magnitude `|a||b|sin(θ)`. It's how you compute torque (`r × F`) and surface
normals.

## Task
In `src/math/Vec3.h`, implement a `Vec3` struct with:
- Fields `x`, `y`, `z` (float)
- Constructors: default (zero), `Vec3(x, y, z)`
- Operators: `+`, `-` (binary and unary negate), `*` (scalar), `/` (scalar), `+=`, `-=`, `*=`
- `dot(const Vec3&) const`
- `cross(const Vec3&) const`
- `length() const`, `lengthSquared() const`
- `normalized() const`
- `operator==`

## Acceptance Criteria
- [ ] `Vec3(1,0,0).cross(Vec3(0,1,0)) == Vec3(0,0,1)`
- [ ] `Vec3(1,0,0).dot(Vec3(1,0,0)) == 1`
- [ ] `Vec3(0,3,4).length() == 5`
- [ ] Cross product is anti-commutative: `a.cross(b) == -(b.cross(a))`

## Hints
- Double-check your cross product component order — it's the single most common bug here (`y*z - z*y` style transposition errors).
