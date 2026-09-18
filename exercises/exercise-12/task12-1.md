# Task 12-1: Sweep and Prune

**Module:** Exercise 12 — Spatial Partitioning at Scale
**Goal:** A broad phase that sorts bodies along one axis and only compares neighbors — no grid tuning required, and cheap to update incrementally frame to frame.

## Concept
**Sweep and Prune (SAP)**: sort all AABBs by their minimum x-coordinate.
Sweep through the sorted list; two AABBs can only overlap if their x-ranges
overlap, so once you hit a body whose min-x is past another's max-x, you
can stop checking further pairs against it (that's the "prune"). Confirm
actual overlap (including the y-axis) only for the x-overlapping candidates.
Because body positions change smoothly frame to frame, the sort is
**almost already sorted** each frame — insertion sort (not a full re-sort)
is typically faster than you'd expect and is the standard choice here.

## Task
- Maintain a persistent sorted array of AABB endpoints (min-x per body is enough for a simple version) across frames
- Each step: update AABBs (Task 6-1), re-sort using insertion sort (exploit near-sortedness — don't call a general-purpose sort that ignores previous order)
- Sweep the sorted list, emitting candidate pairs where x-ranges overlap, then confirm with a y-range check
- Compare output against your Exercise 6 brute-force/grid results on the same scene (should be the same pair set)

## Acceptance Criteria
- [ ] Candidate pair set matches brute force exactly on a test scene
- [ ] Insertion sort on a near-sorted array (bodies moving smoothly frame to frame) is visibly cheap — count swaps per frame and confirm it's small relative to array size, not sorting from scratch
- [ ] A scene with wildly uneven body sizes (some tiny, some huge) still produces correct results — this is the case that trips up a naively-tuned grid

## Hints
- This works well as a drop-in alternative to Exercise 6's grid — keep both around behind a common interface if you want to A/B them in Task 12-3's benchmark.
