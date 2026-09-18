# Task 4-1: Hooke's Law Spring

**Module:** Exercise 4 — Springs & Particle Constraints
**Goal:** Connect two particles with a force-based spring.

## Concept
Hooke's law: `F = -k * (currentLength - restLength) * direction`, applied
equal and opposite to both endpoints. Usually paired with a damping term
(`- damping * relativeVelocityAlongSpring`) or it oscillates forever (or
blows up with explicit Euler — see Exercise 3).

## Task
- Add a `SpringForceGenerator` (fits the pattern from Task 3-5) connecting two particles: stiffness `k`, `restLength`, `damping`
- Apply the computed force to both endpoints (Newton's third law: equal, opposite)
- Test with one particle fixed (`inverseMass = 0`) and one free, hanging under gravity + spring

## Acceptance Criteria
- [ ] The free particle oscillates around its equilibrium position and settles (with damping > 0)
- [ ] With damping = 0, it oscillates indefinitely without gaining energy (using semi-implicit Euler from Exercise 3)
- [ ] Increasing `k` makes the spring visibly stiffer (faster oscillation, less stretch)

## Hints
- Watch stiffness vs. your fixed timestep: very high `k` with explicit/semi-implicit Euler and a large `dt` will blow up — this is your first hands-on encounter with stiff-system instability, which Exercise 15 (XPBD) directly addresses.
