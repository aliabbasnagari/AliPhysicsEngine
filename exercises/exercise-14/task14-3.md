# Task 14-3: EPA

**Module:** Exercise 14 — 3D Collision Detection
**Goal:** Extend GJK's final simplex (a tetrahedron, once it's confirmed to enclose the origin) into penetration depth and contact normal, using the Expanding Polytope Algorithm.

## Concept
When GJK confirms intersection, its final tetrahedron simplex is *inside*
the Minkowski difference, but its faces aren't necessarily on the
difference's actual boundary — so it doesn't yet tell you penetration
depth. EPA repeatedly: finds the tetrahedron/polytope face closest to the
origin, computes a new support point in that face's normal direction,
and — if that new point expands the polytope meaningfully — adds it,
replacing the face with new faces to the new point (like a 3D convex hull
expansion). This continues until the closest face stops improving
(within a tolerance); that face's normal and distance from the origin are
your penetration normal and depth.

## Task
- Implement EPA taking GJK's final simplex (Task 14-2) as a starting polytope
- Maintain a polytope as a list of triangular faces; each iteration, find the face closest to the origin, get a new support point along that face's normal, check if it meaningfully expands the polytope (if the new support point's distance along the normal barely exceeds the face's distance, you've converged)
- On convergence, return the closest face's normal (contact normal) and distance (penetration depth)
- Test against Task 14-1's sphere-vs-sphere (where you already know the correct analytical answer) as a correctness cross-check, then against box-vs-box and box-vs-sphere cases

## Acceptance Criteria
- [ ] For a sphere-vs-sphere overlap, EPA's computed normal and depth match the analytical Task 14-1 answer closely
- [ ] For two overlapping boxes, the reported normal matches whichever face is actually the shallowest penetration direction (verify against a few hand-reasoned cases, e.g. axis-aligned boxes overlapping only slightly along one axis)
- [ ] EPA terminates (doesn't infinite-loop) even for deep/awkward penetrations — add and test an iteration cap as a safety net regardless of convergence tolerance
- [ ] You can articulate why GJK alone isn't enough and what specifically EPA adds (write it as a comment — this is a common “I copied the algorithm but don't actually get it” gap worth closing explicitly)

## Hints
- Numerical robustness (near-degenerate faces, nearly-coplanar points) is where EPA implementations tend to break in practice — if you hit weird crashes/infinite loops on specific shape configurations, that's normal territory for this algorithm, not necessarily a fundamental logic bug; add epsilon guards rather than assuming your core algorithm is wrong first.
