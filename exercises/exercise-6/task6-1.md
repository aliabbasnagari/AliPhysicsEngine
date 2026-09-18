# Task 6-1: AABB Computation

**Module:** Exercise 6 — Broad-Phase Collision Detection
**Goal:** Compute an axis-aligned bounding box for every body shape, updated each frame.

## Concept
An AABB (axis-aligned bounding box) is the smallest box, aligned to world
axes, that fully contains a shape. It's cheap to compute and cheap to test
for overlap (`4` comparisons), which is exactly what broad phase needs.

## Task
- Add an `AABB` type (`min: Vec2`, `max: Vec2`) with an `overlaps(const AABB&) const` test
- Compute an AABB per body per step:
  - Circle: `min = position - (radius, radius)`, `max = position + (radius, radius)`
  - Box (rotated!): transform all 4 corners by the body's rotation, then take min/max — do **not** just use the unrotated half-extents, a rotated box's AABB is larger than its own bounding box at rotation 0
- Store or recompute this each frame (whichever is simpler for now — caching/optimizing this is Exercise 16's job, not this one's)

## Acceptance Criteria
- [ ] `AABB::overlaps` correctly returns true/false for known overlapping/non-overlapping cases
- [ ] A box body rotated 45° has a visibly larger AABB than at rotation 0 (draw it with your debug draw to check)
- [ ] Two circles just touching produce AABBs that just touch (no gap, no overlap, at the boundary)

## Hints
- Draw the computed AABBs on screen temporarily (a colored rectangle per body) — it's the fastest way to catch a wrong-corner or forgot-to-rotate bug.
