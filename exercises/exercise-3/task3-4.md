# Task 3-4: Drag / Damping

**Module:** Exercise 3 — Particles & Integrators
**Goal:** Add air resistance so particles settle instead of oscillating/falling forever, and understand a nonlinear force term.

## Concept
Linear drag: `F_drag = -k * velocity`. Quadratic (more realistic aerodynamic)
drag: `F_drag = -k * |velocity| * velocity` (opposes velocity direction,
scales with speed squared). Both remove energy from the system over time.

## Task
- Add a drag force to your semi-implicit Euler particle (pick linear or quadratic, or expose both)
- Expose the drag coefficient as a tunable value (hardcoded constant is fine for now — a debug UI slider is a nice-to-have, not required)
- Verify a particle launched with initial velocity and no gravity gradually slows to a stop rather than moving forever

## Acceptance Criteria
- [ ] A particle with only drag (no gravity) asymptotically approaches zero velocity, never reverses direction
- [ ] Increasing the drag coefficient visibly makes it stop faster
- [ ] Drag force direction always opposes current velocity (verify by logging or eyeballing the trajectory)

## Hints
- Quadratic drag needs `velocity.normalized() * speed*speed`, not `velocity*velocity` (that's not a defined vector operation) — be precise about what "squared" applies to.
