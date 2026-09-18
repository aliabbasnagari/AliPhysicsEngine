# Task 13-2: Quaternion-Based Orientation Integration

**Module:** Exercise 13 — 3D Math & Rigid Bodies
**Goal:** Integrate angular velocity into orientation using quaternions, avoiding gimbal lock and the awkwardness of 3D Euler angles.

## Concept
Angular velocity `ω` (a `Vec3`, axis = rotation axis, magnitude = rate) updates
orientation via the quaternion derivative: treat `ω` as a "pure" quaternion
`(ωx, ωy, ωz, 0)`, then `q_new = normalize(q + 0.5 * (ω_quat * q) * dt)`
(semi-implicit Euler style, matching Exercise 3's approach — compute this
derivative, add it scaled by `dt`, then **always renormalize**, since this
update doesn't preserve unit length exactly).

## Task
- Add `Vec3 angularVelocity` to your 3D rigid body, alongside the `Quaternion orientation` from Task 1-4
- Implement the integration step above, renormalizing every step (unlike the "don't force-normalize every operator" guidance in Task 1-4 — this integration step specifically needs it, since small numerical error accumulates every frame)
- Apply a constant angular velocity to a test body (no torque yet) for many seconds and confirm it keeps rotating smoothly, without drifting off-axis or degenerating

## Acceptance Criteria
- [ ] A body spun around a single fixed axis keeps rotating around that same axis indefinitely (no visible drift onto other axes) — this specifically catches accumulated normalization error
- [ ] `orientation.rotate(v)` (from Task 1-4) applied to a test vector traces out the expected rotation path over time
- [ ] Quaternion magnitude stays extremely close to 1 over a long run (log it periodically and confirm) — confirms renormalization is actually happening every step, not just once at setup

## Hints
- This is the task where a Task 1-4 bug (wrong multiplication order, or a `rotate()` that doesn't match `toMat4()`) tends to surface as "it kind of rotates but wobbles weirdly" — if that happens, go back and re-verify Task 1-4's acceptance criteria in isolation before debugging this task further.
