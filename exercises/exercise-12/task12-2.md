# Task 12-2: Dynamic AABB Tree (BVH)

**Module:** Exercise 12 — Spatial Partitioning at Scale
**Goal:** A hierarchical bounding volume structure that scales well to large, unevenly distributed worlds — what Box2D and most production engines actually use for broad phase.

## Concept
A binary tree where every leaf is a body's (slightly padded/"fattened")
AABB, and every internal node's AABB is the union of its children's. To
find candidate pairs, query the tree for anything overlapping each body's
AABB, descending only into subtrees whose AABB overlaps the query. Updates
are **incremental**: when a body moves outside its fattened leaf AABB,
remove and reinsert just that leaf, not the whole tree.

## Task
- Implement tree insertion: choose where to insert a new leaf by picking, at each internal node, whichever child insertion produces the smaller AABB-area increase (a standard, simple heuristic — you don't need a perfectly balanced tree)
- Implement removal and an `update(body)` that only touches the moved body's leaf and ancestors (re-insert only if the body has left its fattened AABB — this "fattening" with a margin is what avoids re-inserting on every tiny movement)
- Implement a query: given an AABB, return all leaf bodies whose AABB overlaps it (recursive descent, pruning subtrees whose AABB doesn't overlap)
- Generate the world's candidate pair list by querying the tree with each body's own AABB (dedupe pairs as in Exercise 6)

## Acceptance Criteria
- [ ] Candidate pair set matches brute force exactly on a test scene
- [ ] A body moving smoothly (not teleporting) triggers tree restructuring rarely, not every frame (log/count re-insertions vs. total updates)
- [ ] A scene with a large open world and a small cluster of many overlapping bodies in one corner performs well (this is the "uneven distribution" case a fixed grid struggles with) — verify with a comparison in Task 12-3
- [ ] Tree depth stays reasonable (not degenerating into a linked list) for a scene with a few hundred bodies

## Hints
- Draw the tree's internal node AABBs (different color per depth level) temporarily — it's the best way to see whether your insertion heuristic is producing a sensible, balanced-ish tree or a pathological one.
