# Task 5-1: RigidBody Data

**Module:** Exercise 5 — Rigid Body Fundamentals
**Goal:** Extend your particle from Exercise 3 into a full 2D rigid body: position + orientation, linear + angular velocity.

## Concept
A rigid body adds rotation on top of a particle: an orientation angle
`θ` (2D — just one number, unlike 3D which needs a quaternion), and angular
velocity `ω` (also one number in 2D: radians/sec, positive = counter-clockwise
by convention — pick one and document it).

## Task
In `src/physics/RigidBody.h`, define (or extend your Exercise 3 type):
- `Vec2 position`, `float rotation` (radians)
- `Vec2 linearVelocity`, `float angularVelocity`
- `float mass`, `float inverseMass`
- `float inertia`, `float inverseInertia` (leave inertia computation to Task 5-2 — for now, accept it as a constructor parameter)
- A `shapeType` tag (circle, box — enough for Exercise 7) and shape-specific data (radius, or half-extents)
- A way to get a world-space transform (position + rotation) usable by your debug draw from Exercise 2

## Acceptance Criteria
- [ ] A `RigidBody` can be constructed as a circle or a box with explicit mass and inertia
- [ ] `inverseMass == 0 && inverseInertia == 0` marks a static/kinematic body (never moved by forces/torques) — document this convention, you'll use it constantly
- [ ] Drawing a box body at a non-zero rotation via your Exercise 2 debug draw shows it rotated correctly

## Hints
- Keep `RigidBody` a fairly flat, simple struct/class for now — Exercise 16 revisits its memory layout for performance; don't over-engineer that here.
