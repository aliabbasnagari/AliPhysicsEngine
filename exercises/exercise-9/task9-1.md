# Task 9-1: Persistent Contact Manifolds

**Module:** Exercise 9 — Stable Contacts & Solver Quality
**Goal:** Track the *same* contact across frames (not regenerate from scratch every step) so accumulated impulses (Task 9-2) have something to carry forward.

## Concept
Right now, every frame you regenerate manifolds fresh. For warm starting to
work, you need to recognize "this frame's contact point 0 between body A
and body B is the same physical contact as last frame's" — usually via a
stable contact **ID** (e.g. derived from which edges/vertices are involved,
not raw position, since position shifts slightly frame to frame).

## Task
- Give each contact point a persistent ID (a simple scheme: hash/combine the feature indices involved — e.g. which edge or vertex of each shape — rather than trying to match by proximity)
- Cache manifolds per body pair across frames (a map from `(bodyA, bodyB)` pair to their current manifold) instead of rebuilding a fresh throwaway list every step
- When updating a cached manifold each frame: match new contact points to old ones by ID, carrying forward any per-point data you'll need in Task 9-2; drop points that no longer exist, add new ones that appeared

## Acceptance Criteria
- [ ] For a body resting motionless on the floor, the same contact ID(s) persist frame over frame (verify by logging IDs for a few seconds)
- [ ] A body sliding along the floor shows IDs changing only when it actually crosses onto a new feature (e.g. a new floor segment), not every single frame
- [ ] The manifold cache doesn't leak — pairs that stop overlapping (confirmed via broad phase, Exercise 6) have their cached manifold removed

## Hints
- This is bookkeeping, not new physics — resist adding any new resolution logic here. Task 9-2 is where the payoff (warm starting) happens.
