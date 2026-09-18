# Task 9-3: Sequential Impulse Solver Loop

**Module:** Exercise 9 — Stable Contacts & Solver Quality
**Goal:** Formalize the solver into a proper iterative loop over *all* contacts, run multiple times per step — the same relaxation idea as Exercise 4's rope constraints, now applied to collision impulses.

## Concept
Resolving each contact once, independently, doesn't account for contacts
influencing each other (resolving contact A-B can violate contact B-C in a
stack). Sequential impulse solving iterates over *all* active contacts
multiple times per step (Gauss-Seidel style, same pattern as your rope in
Exercise 4), converging toward a mutually consistent solution.

## Task
- Restructure your solver into: for `N` iterations (start around 4-10, make it tunable), loop over every active contact manifold and apply/update its impulse (using the warm-started, accumulated-clamping approach from Task 9-2)
- Positional correction (Task 8-2) can either run once after all velocity iterations, or as its own small iteration loop (a common, effective approach: a handful of position-correction passes too) — pick one and be consistent
- Measure/compare stack stability at low iteration counts (1-2) vs higher (10+) on the same scene

## Acceptance Criteria
- [ ] Increasing iteration count visibly improves stack stability/reduces jitter on a hard scene (e.g. 6+ stacked boxes)
- [ ] At very low iteration count (1), the same scene visibly struggles (sinks, jitters, or topples) — confirms the loop is doing real work, not just wasted cycles
- [ ] Solver iteration count is a single tunable value, not scattered across multiple hardcoded loops

## Hints
- This is a natural point to add an ImGui slider (from your Exercise 2 debug draw work) for iteration count, so you can feel the stability/performance tradeoff live instead of recompiling to test different values.
