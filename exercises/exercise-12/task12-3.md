# Task 12-3: Benchmark Comparison

**Module:** Exercise 12 — Spatial Partitioning at Scale
**Goal:** Actually measure the four broad-phase approaches you've now built (brute force, grid, SAP, BVH) against each other, instead of assuming which is "best."

## Task
- Build a benchmark harness: spawn a configurable number of bodies (try 50, 500, 2000+) in a few different distributions (evenly spread, tightly clustered, mixed sizes) and time each broad-phase implementation's pair-generation step in isolation (exclude rendering, narrow phase, solving)
- Record per-implementation: time per step, candidate pair count produced, and (for BVH/grid) memory/structure overhead if easy to measure
- Present the results somewhere durable — a markdown table in this exercise's folder (e.g. `exercises/exercise-12/results.md`, your own file, not required reading for later exercises) or logged output is fine, whatever you'll actually refer back to

## Acceptance Criteria
- [ ] You have actual numbers, not assumptions, for how each approach scales as body count grows
- [ ] You can identify at least one scenario where brute force is fine (don't assume it's always worst — for very small `n` it often wins due to lower constant overhead)
- [ ] You can identify at least one scenario where the grid underperforms SAP/BVH (the uneven-distribution case)
- [ ] You've picked a default broad phase for the rest of the engine going forward, with a one-sentence justification based on your own measurements

## Hints
- This is the first "did I actually make things faster" checkpoint in the curriculum — the instinct to measure before/after, rather than assume, is exactly the mindset Exercise 16 leans on much harder.
