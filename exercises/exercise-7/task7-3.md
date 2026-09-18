# Task 7-3: Polygon SAT

**Module:** Exercise 7 — Narrow-Phase Collision Detection
**Goal:** General convex polygon vs polygon collision using the Separating Axis Theorem — the workhorse algorithm behind most 2D physics engines' narrow phase.

## Concept
**Separating Axis Theorem**: two convex shapes are *not* colliding if there
exists any axis onto which their projections don't overlap. For polygons,
it's sufficient to test the axes perpendicular to each edge of both
polygons. If every axis shows overlap, the shapes intersect; the axis with
the *smallest* overlap gives you the separation normal and penetration
depth (this is also how you should implement box-vs-box, treating boxes as
4-vertex polygons — no separate box-box routine needed).

## Task
- Represent a convex polygon as an ordered list of world-space vertices (a box is just a 4-vertex case)
- Implement: for each edge of each polygon, compute its outward normal, project both polygons' vertices onto that axis, check for a gap
- Track the axis with minimum overlap (the "minimum translation vector") across all tested axes
- If no separating axis is found, return collision with that minimum-overlap axis as the normal and its overlap amount as the depth
- Get the normal's *direction* right (pointing from A to B) — SAT gives you the axis, not automatically the correct sign

## Acceptance Criteria
- [ ] Two separated polygons correctly report no collision
- [ ] Two overlapping boxes (as 4-vertex polygons) report a normal matching one of the box faces, with a sensible penetration depth
- [ ] A more complex convex polygon (e.g. a pentagon) vs a box works correctly
- [ ] Rotating one shape slowly through another produces a continuously-changing, correct normal (no sudden flips except at genuine face-normal transitions)

## Hints
- Test this in isolation with hardcoded polygon vertices and printed results before wiring it into the live simulation — SAT bugs are much easier to find with known, static inputs than with tumbling bodies on screen.
