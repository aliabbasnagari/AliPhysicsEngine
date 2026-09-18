# Task 1-3: Mat4

**Module:** Exercise 1 — Math Foundations
**Goal:** A 4x4 matrix type for transforms (used for rendering and later for 3D rigid body transforms).

## Concept
A 4x4 matrix in homogeneous coordinates can represent translation, rotation
and scale in a single multiplicable object. Composing transforms is matrix
multiplication; applying a transform to a point is matrix-vector
multiplication.

## Task
In `src/math/Mat4.h`, implement a `Mat4` struct with:
- Storage for 16 floats (decide row-major or column-major and document it — this matters once you feed it to OpenGL)
- `identity()` static factory
- `operator*` for `Mat4 * Mat4` (composition)
- A way to transform a `Vec3` as a point (i.e. treats it as `(x,y,z,1)`) and as a direction (treats it as `(x,y,z,0)`, ignores translation)
- Static factories: `translation(Vec3)`, `scale(Vec3)`, `rotationX(radians)`, `rotationY(radians)`, `rotationZ(radians)`
- `transpose() const`

## Acceptance Criteria
- [ ] `Mat4::identity() * v == v` for any point `v`
- [ ] `Mat4::translation({1,2,3})` transforms `(0,0,0)` to `(1,2,3)`
- [ ] Composing `translation(T) * rotation(R)` and applying to a point matches doing rotation then translation manually
- [ ] `rotationZ(pi/2)` transforms `(1,0,0)` to approximately `(0,1,0)` (or `(0,-1,0)` depending on your handedness/direction convention — just be consistent and document it)

## Hints
- You do not need a general inverse yet — that can wait until you actually need it (camera work, later joints). Don't build it speculatively.
