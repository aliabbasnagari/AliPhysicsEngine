# Task 5-3: Force, Torque & Angular Integration

**Module:** Exercise 5 — Rigid Body Fundamentals
**Goal:** Apply forces at arbitrary points (not just the center of mass) and integrate both linear and angular motion.

## Concept
A force applied off-center produces both linear acceleration *and* torque:
`τ = r × F`, where `r` is the vector from center of mass to the application
point (2D cross product of two vectors yields a scalar — the z-component of
the 3D cross product). Angular integration mirrors linear (semi-implicit
Euler from Exercise 3): `angularVelocity += angularAcceleration * dt;
rotation += angularVelocity * dt;`.

## Task
- Add `applyForce(Vec2 force)` (center of mass, no torque) and `applyForceAtPoint(Vec2 force, Vec2 worldPoint)` (computes and accumulates torque too) to `RigidBody`
- Add a `torqueAccumulator` (float) alongside the linear `forceAccumulator` from Exercise 3
- Extend `integrate(dt)` to also update `angularVelocity` and `rotation` from `torqueAccumulator * inverseInertia`, then clear both accumulators
- Test: apply a force at a body's edge (not center) and confirm it both moves *and* spins

## Acceptance Criteria
- [ ] A force applied exactly at the center of mass produces zero torque (pure translation, no spin)
- [ ] The same force applied at an edge produces visible rotation
- [ ] A body with only torque applied (no net force) spins in place without translating
- [ ] Reversing the force application point (other side of center) reverses the spin direction

## Hints
- 2D cross product for torque: `torque = r.x * F.y - r.y * F.x` (scalar, not `Vec2::cross` from Exercise 1, which was 3D) — you may want a small 2D-specific cross helper.
