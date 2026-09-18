# Task 4-2: Distance Constraint

**Module:** Exercise 4 — Springs & Particle Constraints
**Goal:** An alternative to springs for rigid connections: directly correcting positions to satisfy a distance constraint, rather than applying a force.

## Concept
A spring is "soft" no matter how high you crank `k` before it becomes
unstable. A **position-based distance constraint** instead directly moves
both particles after integration so the distance between them equals a
target, splitting the correction by inverse mass (heavier particles move
less). This is the core operation of position-based dynamics (you'll
generalize it in Exercise 15).

## Task
- Implement `solveDistanceConstraint(Particle& a, Particle& b, float targetDistance)`:
  compute the current separation vector, the error (`currentDistance - targetDistance`),
  and move each particle along that direction proportional to its inverse mass
  share (`invMass / (invMassA + invMassB)`)
- Call this after integration, once per step (you'll add iteration count in Task 4-3)
- Test on two particles, one fixed

## Acceptance Criteria
- [ ] The free particle stays at (approximately) the target distance from the fixed one every frame, unlike the spring which oscillates around it
- [ ] A particle with `inverseMass = 0` never moves from a constraint solve, regardless of the other particle's mass
- [ ] Two particles of equal mass, both free, split the correction 50/50

## Hints
- This is a *positional* correction, not a force — it happens after `integrate()`, not as part of the force accumulator from Task 3-5.
