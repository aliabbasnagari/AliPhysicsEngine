# Task 6-3: Spatial Grid Broad Phase

**Module:** Exercise 6 — Broad-Phase Collision Detection
**Goal:** Replace `O(n^2)` pair generation with a uniform spatial grid, so only nearby bodies are ever compared.

## Concept
Divide the world into fixed-size grid cells. Insert each body into every
cell its AABB overlaps (a body can span multiple cells). Only generate
candidate pairs between bodies that share at least one cell. With a
reasonable cell size, this turns `O(n^2)` into roughly `O(n)` for evenly
distributed bodies.

## Task
- Implement a grid (a hash map from cell coordinate `(x, y)` to a list of body indices is the simplest approach; you don't need a dense 2D array)
- Pick a cell size (start with something close to your average body size) and make it easy to tune
- Each step: clear the grid, insert every body's AABB into all cells it overlaps, then generate candidate pairs only from bodies sharing a cell (still dedupe — the same pair can appear via multiple shared cells)
- Compare output against Task 6-2's brute-force result on the same scene — the candidate pair *sets* should match (grid can't be looser in coverage, only more efficient)

## Acceptance Criteria
- [ ] For a test scene, the grid's candidate pair set exactly matches the brute-force set (correctness first)
- [ ] With many bodies spread across a large world (a few hundred, sparse), the grid produces measurably fewer pair-vs-pair comparisons than brute force (add a simple counter to compare)
- [ ] Changing cell size still produces correct (if not necessarily equally fast) results — verifies your dedup logic isn't accidentally cell-size-dependent

## Hints
- A cell size much smaller than your bodies means each body spans many cells (more insertion overhead); much larger means every body ends up in the same cell or two (back to brute force). This tradeoff is why Exercise 12 covers other structures (BVH, sweep-and-prune) that don't need manual tuning.
