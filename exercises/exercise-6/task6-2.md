# Task 6-2: Brute-Force Broad Phase

**Module:** Exercise 6 — Broad-Phase Collision Detection
**Goal:** Generate a candidate pair list by testing every body against every other body's AABB — simple, correct, and a useful baseline before optimizing.

## Task
- Implement a function that, given all bodies in the world, returns a list of pairs `(bodyA, bodyB)` whose AABBs overlap (from Task 6-1)
- Naive nested loop is fine and expected here: `O(n^2)` pairs checked
- Skip pairs where both bodies are static (two immovable bodies never need collision response)
- Skip self-pairs and duplicate pairs (`(A,B)` and `(B,A)` are the same pair — only emit one)
- Call this each step in `PhysicsWorld::step()`, before narrow phase (which doesn't exist yet — for now, just log/count the candidate pairs found)

## Acceptance Criteria
- [ ] Spawning bodies that visually overlap produces candidate pairs; bodies far apart don't
- [ ] No duplicate pairs, no self-pairs, no static-static pairs in the output
- [ ] You can print the pair count per frame and watch it change sensibly as bodies move apart/together

## Hints
- This is intentionally the "dumb" version — the point is a correct reference implementation you can compare Task 6-3's optimized version against (same input, same pair set, different algorithm).
