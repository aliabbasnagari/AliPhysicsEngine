# Task 14-2: GJK

**Module:** Exercise 14 — 3D Collision Detection
**Goal:** Implement the Gilbert-Johnson-Keerthi algorithm to test whether two arbitrary convex shapes intersect.

## Concept
GJK works on the **Minkowski difference** of two convex shapes (`A - B`):
two shapes intersect iff their Minkowski difference contains the origin.
Rather than computing the full Minkowski difference, GJK queries it via a
**support function**: `support(A, B, direction) = furthestPoint(A, direction) - furthestPoint(B, -direction)`,
where `furthestPoint(shape, direction)` returns the shape's vertex furthest
in that direction (trivial for a sphere: center + radius*direction; for a
box: whichever corner has the largest dot product with direction, evaluated
per-axis; for a general polytope: iterate vertices).

The algorithm iteratively builds a **simplex** (up to 4 points in 3D: point,
line, triangle, tetrahedron) from support points, each time picking a new
search direction that moves toward the origin, until either the simplex
contains the origin (intersection found) or no progress can be made
(no intersection). This is the algorithm's core loop and the part worth
understanding deeply rather than copying — work through a 2D-simplified
version by hand on paper first if the 3D case is hard to visualize.

## Task
- Implement `support(shapeA, shapeB, direction)` for your shape types (sphere, box at minimum; a general convex polytope with a vertex list if you want to go further)
- Implement the GJK loop: start with an arbitrary direction, build up the simplex, use the appropriate case handling per simplex size (point/line/triangle/tetrahedron) to either find a new direction or detect the origin is enclosed
- Return a simple boolean (intersecting or not) initially — don't try to extract penetration info yet, that's Task 14-3's job, and mixing the two concerns while debugging GJK itself will make bugs harder to isolate
- Test extensively with known-separated and known-overlapping sphere/box pairs (start with symmetric, easy cases before rotated/offset ones)

## Acceptance Criteria
- [ ] Correctly reports true/false for a wide range of hand-constructed test cases (both spheres, both boxes, sphere-vs-box), including edge/corner/face grazing cases where shapes are just barely touching
- [ ] Correctly reports overlap for two boxes rotated at arbitrary angles that visually intersect
- [ ] Matches the simpler Task 14-1 sphere-vs-sphere test's results exactly on the same inputs — this is your correctness cross-check before trusting GJK on shapes you don't have another test for

## Hints
- This is the hardest task in the curriculum so far. Do not skip writing isolated unit-style tests with hardcoded known shapes before wiring it into the live simulation — debugging GJK from a tumbling 3D scene on screen is extremely painful compared to debugging it against a fixed, known test case.
