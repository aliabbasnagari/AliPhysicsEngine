# Task 3-3: Verlet Integration

**Module:** Exercise 3 — Particles & Integrators
**Goal:** A third integration scheme, storing position history instead of velocity — the basis for the rope/cloth work in Exercise 4.

## Concept
**Position (Störmer-)Verlet** derives velocity implicitly from two stored
positions: `newPosition = 2*position - oldPosition + acceleration*dt*dt`.
No explicit velocity variable is needed. It's popular for constraint-based
simulation (ropes, cloth) because constraints can directly nudge positions
without having to also patch up a velocity variable.

## Task
- Implement a Verlet-integrated particle variant (can be the same type with a third mode, or a distinct one if that's cleaner for you)
- Store `oldPosition` instead of/alongside `velocity`
- If you need an instantaneous velocity for anything (e.g. drag in Task 3-4), derive it as `(position - oldPosition) / dt`
- Compare against semi-implicit Euler on the same falling/oscillating test case

## Acceptance Criteria
- [ ] A Verlet particle under constant gravity falls with the same trajectory as the semi-implicit Euler version (within numerical tolerance)
- [ ] You can compute an approximate velocity from position history and it matches the semi-implicit Euler particle's velocity reasonably closely
- [ ] You understand (and can state in a comment) why Verlet is convenient for constraint solving: you can just set `position` directly and the "velocity" adapts automatically next step

## Hints
- Initialize `oldPosition` correctly on spawn (e.g. `oldPosition = position - velocity*dt` if you're starting from a known initial velocity) — an ad-hoc identical value here produces a stationary-looking first frame.
