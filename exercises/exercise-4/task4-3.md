# Task 4-3: Rope

**Module:** Exercise 4 — Springs & Particle Constraints
**Goal:** Chain many particles with distance constraints to build a rope — and learn why constraint solving needs multiple iterations.

## Concept
A rope is a line of particles, each connected to its neighbor by a distance
constraint. Solving each constraint once per step isn't enough once you
have a *chain*: fixing constraint 1-2 can un-satisfy constraint 2-3.
Relaxation (Gauss-Seidel style: solve all constraints, repeat N times per
step) converges toward a solution that satisfies all constraints
simultaneously.

## Task
- Build a rope: N particles in a line, each pair of neighbors linked by the distance constraint from Task 4-2, first particle's `inverseMass = 0` (anchored)
- Integrate all particles each step (gravity via Task 3-5's generator), then run the full set of pairwise constraints multiple times (e.g. 8-15 iterations) per step
- Expose iteration count as a tunable value and observe the rope sagging/stretching less as iteration count increases

## Acceptance Criteria
- [ ] The rope hangs and swings convincingly under gravity, anchored at one end
- [ ] With 1 solver iteration, the rope visibly over-stretches under its own weight; increasing iterations tightens it up
- [ ] Grabbing/moving the anchor point (or perturbing a particle) propagates realistically along the chain over a few frames

## Hints
- Use Verlet integration (Task 3-3) here if you haven't already — it plays especially nicely with this position-correction style of constraint solving.
