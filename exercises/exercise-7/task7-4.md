# Task 7-4: Contact Manifold Generation

**Module:** Exercise 7 — Narrow-Phase Collision Detection
**Goal:** Unify Tasks 7-1 through 7-3 behind one interface, and generate *multiple* contact points where needed (not just one) — required for stable box-on-box resting contact in Exercise 8.

## Concept
A single contact point is enough for circle-circle, but two boxes resting
flat on each other actually touch along an *edge*, which needs (up to) two
contact points to resolve without rotating unrealistically. A **contact
manifold** holds: the two bodies involved, the shared normal, and 1-2
contact points each with their own penetration depth.

## Task
- Define a `Manifold` (or `Contact`) struct: `bodyA`, `bodyB`, `normal`, and up to 2 `ContactPoint { position, penetration }`
- Dispatch to the right narrow-phase routine (Task 7-1/7-2/7-3) based on the shape types of the pair
- For polygon-polygon (and box-polygon), when the deepest-penetrating feature is edge-vs-edge (not vertex-vs-edge), clip the incident edge against the reference edge's side planes to produce up to 2 contact points (this is the standard "reference/incident edge clipping" step used after SAT finds the minimum-penetration axis)
- For circle-anything, a single contact point is correct and sufficient — don't force 2 points where there's only ever 1

## Acceptance Criteria
- [ ] Calling one function with any two bodies from the broad-phase pair list (Exercise 6) returns a correct `Manifold` (or "no collision")
- [ ] A box resting flat on another box produces 2 contact points along the shared edge, not 1
- [ ] A box corner touching another box's face produces exactly 1 contact point
- [ ] Circle-involving collisions always produce exactly 1 contact point

## Hints
- You don't need to resolve anything yet (that's Exercise 8) — for now, just visualize the manifold: draw the normal as an arrow and each contact point as a dot, and manually verify they look right across several shape combinations before moving on.
