# Task 11-1: Ray vs Shape

**Module:** Exercise 11 — Continuous Collision Detection & Raycasting
**Goal:** Cast a ray into the world and find the nearest shape it hits — useful standalone (mouse picking, line of sight) and the core primitive CCD builds on.

## Concept
- Ray vs circle: solve the quadratic formed by substituting the ray parametric equation into the circle equation; the smallest non-negative root within the ray's length is your hit
- Ray vs AABB: the "slab method" — compute the `t` interval where the ray is within each axis's slab, intersect the intervals across x and y; a valid hit exists if the intersected interval is non-empty and non-negative
- Ray vs polygon: test the ray against each edge as a line segment, keep the nearest valid hit

## Task
- Implement `raycast(origin, direction, maxDistance, body) -> optional<HitInfo{point, normal, distance}>` for circle, AABB/OBB, and polygon shapes
- Implement a `raycastWorld(origin, direction, maxDistance) -> optional<HitInfo>` that tests against all bodies (use your Exercise 6 broad phase to skip far-away bodies first) and returns the *nearest* hit
- Add a debug feature: cast a ray from the mouse position into the world each frame and highlight whatever it hits (great way to verify all shape types visually)

## Acceptance Criteria
- [ ] A ray that should miss a shape correctly reports no hit
- [ ] A ray hitting a circle reports a hit point on the circle's surface (not center) and an outward-pointing normal
- [ ] A ray hitting a rotated box correctly accounts for the rotation (test at several angles)
- [ ] `raycastWorld` returns the nearest of several overlapping/aligned candidate hits, not just the first one found

## Hints
- Ray vs OBB is easiest by transforming the ray into the box's local space (inverse rotate/translate) and doing an axis-aligned slab test there, then transforming the hit point back — same trick as Task 7-2's circle vs OBB.
