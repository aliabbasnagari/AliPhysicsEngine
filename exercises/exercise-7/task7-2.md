# Task 7-2: Circle vs AABB/OBB

**Module:** Exercise 7 — Narrow-Phase Collision Detection
**Goal:** Detect and resolve-info for a circle against an axis-aligned or oriented box.

## Concept
Find the closest point on the box to the circle's center (clamp the
circle's center, transformed into the box's local space, to the box's
half-extents), then treat that closest point like a circle-circle test: if
the distance from the circle's center to that closest point is less than
the circle's radius, they collide.

## Task
- Implement circle vs AABB first (box is axis-aligned — no local-space transform needed, just clamp in world space)
- Extend to OBB (oriented box): transform the circle's center into the box's local space (inverse of the box's rotation) before clamping, then transform the resulting closest point back to world space
- Return the same contact info shape as Task 7-1: normal, depth, contact point
- Handle the case where the circle's center is *inside* the box (clamped point equals center — pick a fallback normal, e.g. push out along the axis of least penetration)

## Acceptance Criteria
- [ ] A circle approaching a box's face reports a normal perpendicular to that face
- [ ] A circle approaching a box's corner reports a normal pointing away from that corner, not a face normal
- [ ] A circle centered inside the box doesn't crash and reports *some* reasonable separating normal/depth
- [ ] Works correctly for a box rotated at an arbitrary angle, not just axis-aligned

## Hints
- Draw the "closest point" your algorithm computes as a small dot — if it's not visually on the box's boundary when it should be, your clamp or space transform is wrong.
