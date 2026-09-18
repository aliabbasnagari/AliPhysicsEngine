# Task 10-2: Revolute Joint

**Module:** Exercise 10 — Joints & Constraints
**Goal:** A pin/hinge joint — two bodies share a single point and can rotate freely around it (think: a door hinge, a pendulum arm, a ragdoll elbow).

## Concept
A revolute joint constrains the world-space position of an anchor point to
be identical on both bodies (zero relative distance, not a fixed nonzero
one like Task 10-1) while leaving relative rotation free. It's a 2D point
constraint: solve for a 2D impulse (not a scalar like the distance joint)
that drives the relative velocity of the two anchor points to zero.

## Task
- Define a `RevoluteJoint`: two bodies, one local anchor point per body (they should coincide in world space when satisfied)
- Solve as a 2-dimensional velocity constraint (an effective 2x2 mass matrix from both bodies' inverse mass and inverse inertia, similar structure to the distance joint but unconstrained in direction) — or, more simply, run two orthogonal 1D solves (x and y) if you want to avoid building a 2x2 matrix solve for now, at some accuracy cost worth knowing about
- Add positional correction for anchor point drift, same pattern as before
- Test: a box pinned at one corner to a static point, swinging under gravity like a pendulum

## Acceptance Criteria
- [ ] The pinned body swings freely around the anchor point under gravity, like a pendulum
- [ ] The anchor point itself doesn't visibly drift apart over time (positional correction working)
- [ ] Two dynamic bodies pinned to each other (neither static) both move and rotate correctly around their shared point
- [ ] The joint imposes no restriction on relative rotation — the body should be able to spin freely about the pin, only its position relative to the pin is constrained

## Hints
- If you took the "two independent 1D solves" shortcut, come back to this after Exercise 10 if stacking/chained-joint stability bothers you later — a proper 2x2 solve is the more correct approach and worth knowing you took a shortcut here.
