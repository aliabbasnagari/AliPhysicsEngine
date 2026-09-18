# Task 17-2: Island-Based Parallel Solving

**Module:** Exercise 17 — Multithreading & Determinism
**Goal:** Parallelize the constraint solver itself — much harder than broad phase, because contacts within a connected group of bodies genuinely can't be solved independently of each other.

## Concept
An **island** is a maximal group of bodies connected (directly or
transitively) by contacts/joints. Bodies in *different* islands share no
constraints, so their islands can be solved fully independently, in
parallel, with zero risk of a race (this is also naturally where sleeping,
Task 9-4, already operates — you may already have partial island-adjacent
logic there worth reusing/formalizing).

## Task
- Build island detection: union-find (disjoint set) over bodies, unioning any two bodies that share a contact or joint this step, is the standard, simple approach
- Group contacts/joints by their island
- Solve each island's constraints (Exercise 9's solver loop) on its own thread/task, islands running concurrently; within a single island, keep solving sequential (that inner solve genuinely has the ordering dependencies you built in Exercise 8-9)
- Verify: results for a *single* island (single-threaded or run in isolation) are unchanged from before this task — island grouping itself shouldn't alter physics, only how work is scheduled

## Acceptance Criteria
- [ ] A scene with multiple clearly-separated clusters of bodies (e.g. several independent stacks far apart) correctly identifies them as separate islands
- [ ] Physics results (positions/velocities after N steps) match the single-threaded solver closely for the same scene and initial conditions (small floating-point-order differences are expected and fine; a fundamentally different resting configuration is not)
- [ ] Measured speedup on a scene with several large, independent islands
- [ ] A scene that's effectively one giant connected island (e.g. one huge stack) correctly shows little/no speedup — confirms your understanding of *why* this technique has a scene-dependent ceiling, not a bug

## Hints
- This task is a good one to explicitly *not* rush — a genuinely correct island-parallel solver is one of the more sophisticated pieces of engineering in this whole curriculum, and a subtly wrong one (a body's contacts split across two "islands" that should've been one) is a hard bug to spot without hunting for it specifically.
