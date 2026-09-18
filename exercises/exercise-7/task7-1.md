# Task 7-1: Circle vs Circle

**Module:** Exercise 7 — Narrow-Phase Collision Detection
**Goal:** The simplest possible exact collision test — the one every other shape pair eventually reduces toward conceptually.

## Concept
Two circles overlap iff the distance between centers is less than the sum
of radii. Penetration depth = `(rA + rB) - distance`. Normal = the
direction from A to B (normalized).

## Task
In your new `Collision.h`/`.cpp`, implement a function that takes two
circle bodies and returns whether they collide plus, if so:
- Contact normal (unit vector, pick and document a consistent direction, e.g. always A→B)
- Penetration depth
- A contact point (e.g. the midpoint between the two surface points along the normal)

Use `Vec2::length`/`dot` from Exercise 1 — no new math primitives needed.

## Acceptance Criteria
- [ ] Two circles with distance > sum of radii report no collision
- [ ] Two circles with distance < sum of radii report collision, with correct penetration depth (verify against hand-computed values for a couple of test cases)
- [ ] Concentric circles (distance == 0) don't crash (normalize-of-zero-vector case) — decide on and document a fallback normal for this degenerate case

## Hints
- Compare squared distances against squared radius-sum first to early-out without a `sqrt` in the common non-colliding case — a small but standard optimization worth forming as a habit now.
