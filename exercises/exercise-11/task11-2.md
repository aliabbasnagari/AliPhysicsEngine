# Task 11-2: Demonstrate Tunneling

**Module:** Exercise 11 — Continuous Collision Detection & Raycasting
**Goal:** Deliberately reproduce the tunneling problem so you have a concrete, reproducible test case before building the fix in Task 11-3.

## Concept
Discrete collision detection only checks for overlap at discrete sampled
instants (each fixed timestep). If a fast-moving small body's entire
movement in one step carries it from clearly-before a thin wall to
clearly-after it, with no sampled instant actually overlapping the wall,
your existing pipeline (Exercises 6-8) will never detect the collision.

## Task
- Build a specific test scene: a thin static wall (a box with small width) and a small, fast-moving circle launched directly at it with high velocity, using only your existing discrete pipeline (no CCD yet)
- Tune the velocity/timestep/wall thickness until you reliably reproduce the body passing straight through
- Add a way to measure/confirm it happened (e.g. log the body's position each step and confirm it jumped from one side of the wall to the other without ever registering a collision)

## Acceptance Criteria
- [ ] You have a specific, reproducible scene where a body tunnels through a wall using only Exercises 6-8's discrete detection
- [ ] You can articulate why it happens for *this* body/wall/velocity combination (e.g. "the body moves 3x its own diameter per step, and the wall is thinner than that")
- [ ] You've tried the "cheap fix" of just increasing solver/broad-phase precision and confirmed it does NOT fully solve it (only reduces how easily it's triggered) — motivating why CCD (Task 11-3) is a structurally different fix, not just a tuning problem

## Hints
- Keep this exact test scene around — it's your acceptance test for Task 11-3 (same scene, should no longer tunnel once CCD is added).
