# Task 13-3: 3D RigidBody & PhysicsWorld Step

**Module:** Exercise 13 — 3D Math & Rigid Bodies
**Goal:** Bring Tasks 13-1 and 13-2 together into a full 3D rigid body stepped by `PhysicsWorld`, mirroring Exercise 5-4 but in 3D.

## Task
- Assemble a 3D `RigidBody`: `Vec3 position`, `Quaternion orientation`, `Vec3 linearVelocity`, `Vec3 angularVelocity`, mass/inverse mass, local + world inverse inertia tensor (Task 13-1), force/torque accumulators (`Vec3` now, not scalar torque)
- `applyForceAtPoint(Vec3 force, Vec3 worldPoint)`: linear force plus `torque = (worldPoint - position).cross(force)` (a real 3D cross product this time, unlike Task 5-3's 2D scalar version)
- Extend `PhysicsWorld` (or add a parallel 3D world, your call on architecture) to step 3D bodies: apply gravity, integrate linear motion (Exercise 3 patterns), integrate angular motion (Task 13-2), recompute world-space inverse inertia tensor each step (orientation changed)
- Render 3D bodies somehow for visual verification — this doesn't need to be fancy (wireframe boxes/spheres via simple line drawing extended to 3D, or even just printing position/orientation) but you need *some* way to see what's happening; a full 3D renderer isn't required by this task

## Acceptance Criteria
- [ ] A 3D body falls under gravity and can be given an initial angular velocity that persists correctly (per Task 13-2)
- [ ] Applying an off-center force produces both translation and the correct rotation (visually or by checking the resulting angular velocity direction against `r × F` by hand for a test case)
- [ ] A tumbling asymmetric body (e.g. a long box, not a cube) rotates non-uniformly in a way that looks physically plausible — a classic real-world check is the "intermediate axis theorem" (a tumbling object rotated about its *intermediate* moment-of-inertia axis is unstable and will visibly wobble/flip) — you don't have to chase this exactly, but if your box rotates suspiciously uniformly around every axis, that's a sign your tensor isn't actually being used correctly

## Hints
- You do not need 3D collision detection yet (that's Exercise 14) — this task is purely "does gravity + torque + integration work correctly in 3D," tested with non-colliding bodies.
