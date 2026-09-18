# Task 3-2: Semi-Implicit Euler

**Module:** Exercise 3 — Particles & Integrators
**Goal:** Understand *why* virtually every real-time physics engine uses semi-implicit (symplectic) Euler instead of explicit Euler.

## Concept
Semi-implicit Euler updates velocity first, then uses the *new* velocity to
update position: `velocity += acceleration * dt; position += velocity * dt;`.
That one reordering makes it far more numerically stable (energy-conserving
in oscillating systems) than explicit Euler, at the same cost.

## Task
- Add a second integration mode (semi-implicit Euler) to your particle, switchable at runtime (e.g. an enum or a bool passed into `integrate`)
- Build a test case where the difference is visible: a particle on a spring (even a hand-rolled one just for this test, ahead of Exercise 4) or an orbit-like scenario — something oscillating, not just falling
- Run both integrators side by side (two particles, same initial conditions, different integrator) and observe behavior over many seconds

## Acceptance Criteria
- [ ] With a spring-like restoring force, explicit Euler visibly gains energy over time (oscillation amplitude grows, eventually blows up)
- [ ] Semi-implicit Euler on the same setup stays bounded/stable over the same duration
- [ ] You can articulate in one sentence why swapping the update order matters (write it as a code comment)

## Hints
- Don't remove explicit Euler — keep both, so you have a permanent "this is why we don't do it this way" reference.
