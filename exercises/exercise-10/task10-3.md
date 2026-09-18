# Task 10-3: Prismatic Joint

**Module:** Exercise 10 — Joints & Constraints
**Goal:** A slider joint — two bodies can move relative to each other only along a fixed axis (think: a piston, a drawer, an elevator).

## Concept
A prismatic joint constrains relative motion **perpendicular** to a chosen
axis (fixed in one body's local frame) to zero, while allowing free motion
*along* that axis. It also typically locks relative rotation (the two
bodies stay at a fixed relative angle) unless you explicitly want a
rotating slider.

## Task
- Define a `PrismaticJoint`: two bodies, a local axis (defined in one body's frame), an anchor point
- Solve the perpendicular-to-axis constraint as a 1D velocity constraint (project relative velocity onto the perpendicular direction, solve an impulse along that direction — same style as the distance joint's 1D solve, different direction choice)
- Solve the relative-angle constraint (if you're locking rotation) as its own 1D angular velocity constraint
- Optionally: add min/max limits along the slide axis (clamp position, or add a limit impulse when the body reaches an end)

## Acceptance Criteria
- [ ] A body constrained to a vertical prismatic joint on a static anchor falls straight down (or up, under an upward force) and stops moving sideways even when given a sideways push
- [ ] The body's rotation stays locked relative to the axis body throughout (doesn't spin)
- [ ] If you implemented limits: the body stops at the configured min/max and doesn't slide past it

## Hints
- A piston/elevator scene (a box constrained to slide vertically between two limits, pushed by gravity and stopped by the limit) is a satisfying, easy-to-verify test case for this joint.
