# Task 10-1: Distance Joint

**Module:** Exercise 10 — Joints & Constraints
**Goal:** Your first rigid-body joint: keep two anchor points (one per body, not necessarily the center of mass) at a fixed distance — this is Exercise 4's particle distance constraint, generalized to bodies with rotation.

## Concept
Unlike Exercise 4's particles, anchor points here are offset from each
body's center of mass, so correcting the constraint also imparts torque.
You can implement this either as a **velocity constraint** (solve for an
impulse each solver iteration, same style as Exercise 8's contacts) or as a
**positional correction** (same style as Exercise 4/8-2) — a velocity
constraint that also does positional correction (like Exercise 8's
contacts do) is the more robust, standard approach; that's what to build
here.

## Task
- Define a `DistanceJoint`: two bodies, a local anchor point on each (offset from center of mass), a target distance
- Each solver iteration: compute world-space anchor positions (`body.position + rotate(localAnchor, body.rotation)`), the relative velocity of those two world points (including each body's angular velocity contribution, same as Task 8-4), and solve for the impulse along the anchor-to-anchor direction that drives that relative velocity to (approximately) zero at the target distance
- Add positional correction (Task 8-2 style) for the residual distance error
- Register joints on `PhysicsWorld` and solve them in the same iteration loop as contacts (Task 9-3)

## Acceptance Criteria
- [ ] Two bodies linked by a distance joint (one static/anchored) swing like a pendulum, maintaining the target distance
- [ ] The joint correctly imparts torque when the anchor point isn't at the center of mass (e.g. a box joint-anchored at a corner tumbles as it swings, not just translates)
- [ ] Chaining several distance joints reproduces rope-like behavior (compare visually against Exercise 4's particle rope)

## Hints
- If you built the particle distance constraint (Task 4-2) and contact solver (Task 8-1/8-4) well, this task is mostly recombining those two pieces of logic for a new context — resist rewriting either from scratch.
