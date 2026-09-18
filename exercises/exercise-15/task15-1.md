# Task 15-1: Generalized PBD Solver

**Module:** Exercise 15 — Position-Based & Extended Position-Based Dynamics
**Goal:** Formalize Exercise 4's ad-hoc distance-constraint solving into a proper, reusable Position-Based Dynamics solver that can host multiple constraint *types*, not just distance.

## Concept
PBD's loop, in general form: predict new positions (integrate velocities
without applying constraints yet), then repeatedly **project** each active
constraint (move the involved particles' positions to reduce that
constraint's error, weighted by inverse mass — exactly Task 4-2's pattern),
then derive final velocity from the position change (`velocity = (newPosition - oldPosition) / dt`,
Verlet-style, per Task 3-3). The generalization is: define a `Constraint`
interface with a `project()` method, so distance, bending, and volume
constraints (Tasks 15-3/15-4) all plug into the same loop.

## Task
- Define a `Constraint` interface/base with a `project(particles)` method and a way to declare which particles it involves
- Reimplement Exercise 4's distance constraint as one implementation of this interface
- Build the general solver loop: predict positions → for N iterations, project every active constraint → update velocities from position delta
- Re-run your Exercise 4 rope/cloth scenes through this new generalized solver and confirm equivalent behavior to before

## Acceptance Criteria
- [ ] Rope and cloth from Exercise 4 behave the same (or better) when driven through the new generalized solver
- [ ] Adding a brand-new constraint type requires implementing the `Constraint` interface only — no changes to the core solver loop
- [ ] The solver correctly handles particles participating in multiple simultaneous constraints (e.g. a cloth vertex with 4 structural neighbors)

## Hints
- This task is architectural, not new physics — if it feels like busywork, that's somewhat the point: it's paying down the "everything is a special case" debt from Exercise 4 so Tasks 15-3/15-4 are additive instead of rewrites.
