# Evaluation — Exercise 6

**Verdict: PASS.** Tasks 6-1 and 6-2 pass, and the reattempt of Task 6-3 fixes the correctness bug and builds the missing evidence for all three criteria. This was a code and scene review of commit `1ab7d6f`. This shell has no C++ compiler, so I did not build or run the app. The numbers the scene displays, such as the pair counts and the overlap-test ratio, are therefore not something I observed, so check them when you run it.

| Task | Result |
|---|---|
| 6-1 AABB Computation | PASS (one dead check still to fix) |
| 6-2 Brute-Force Broad Phase | PASS |
| 6-3 Spatial Grid Broad Phase | PASS (reattempt) |

## Task 6-1 — AABB Computation
- `AABB` in [AABB.h](../../src/physics/AABB.h) has `min`, `max`, `overlaps`, plus handy `center()` and `size()`. `overlaps` is the standard four-way separation test. It counts touching edges as overlapping, and the comment explains why: broad phase must never miss a pair, and narrow phase rejects false positives. That gives you "two circles just touching produce touching AABBs."
- `computeAABB` in [RigidBody.cpp](../../src/physics/RigidBody.cpp) is correct for both shapes. The circle is `position ± radius`. The box transforms all four corners through `Transform::localToWorld`, then takes min/max. It seeds from the first corner rather than zero, so a body far from the origin is handled correctly. It is recomputed on demand, which the task allows.
- [AABBScene.cpp](../../src/scenes/AABBScene.cpp) covers the visual criteria well:
  - five static boxes at 0, π/8, π/4, 3π/8 and π/2, each with its AABB drawn,
  - a spinning box whose AABB should pulse,
  - three circle pairs at gaps of exactly touching, just apart and just overlapping, coloured red or green by `overlaps`.
- **Still open: the "known cases" check is dead code.** `checkOverlap` computes `actual` and throws it away. It ignores `label` and `expected`, and prints and asserts nothing. The same goes for `s0` and `s45` at the end of `onEnter`: computed, never shown. So the five `overlaps` cases (overlapping, separated on x, separated on y, touching, fully inside) are never checked anywhere. I read the logic by hand and all five come out right, but the scene doesn't show it. This commit did not touch `AABBScene`. Fix: have `checkOverlap` store `actual == expected` and show a "5/5 overlap cases pass" line in ImGui, and show `s0` and `s45` as text so the size difference is a number. It doesn't block, because the visual criteria are met, but it is the one loose end in 6-1.

## Task 6-2 — Brute-Force Broad Phase
- `findPairsBruteForce` in [BroadPhase.cpp](../../src/physics/BroadPhase.cpp) is a correct `O(n²)` loop. Starting `j` at `i + 1` rules out self-pairs and `(A,B)`/`(B,A)` duplicates by construction. It skips static-static pairs. It computes each AABB once up front instead of once per pair.
- `PhysicsWorld::step()` runs it after integration and stores the result in `candidatePairs`, with a `getCandidatePairs()` accessor. `removeRigidBody` also purges any pair that references the removed body, so no dangling pointers.
- [BroadPhaseScene.cpp](../../src/scenes/BroadPhaseScene.cpp) is a thorough demonstration:
  - two groups drift through each other, so the pair count rises, then falls,
  - a chain of three circles where only neighbours touch (expect 2 pairs),
  - isolated bodies that must never pair,
  - overlapping static-static boxes that must be skipped,
  - a static-dynamic overlap that must be kept,
  - a live "Pair list valid" readout that checks for self-pairs, static-static pairs and duplicates.
- Minor: the "expect 2 pairs" cluster only works because the circles touch at exactly one coordinate and `overlaps` is inclusive. A comment saying so would stop it looking like a magic number.

## Task 6-3 — Spatial Grid Broad Phase (reattempt)

Every item from the last review was addressed. The grid now behaves as the task asks.

### Correctness (criterion 1)
- **The overlap filter is in.** After the static check and the dedup, the grid tests `boxes[a].overlaps(boxes[b])` before it emits a pair. The comment explains why: sharing a cell isn't overlap. This fixes the earlier bug where two non-overlapping circles in the same cell got paired.
- **The grid cannot miss a pair.** `floor(x / cellSize)` is monotone, and so is the clamp in `toCell`. If two AABBs overlap, their cell ranges overlap, so the pair is always reached through at least one shared cell. Touching AABBs share a cell too, because the equal edge coordinate maps to the same cell. With the filter, the grid emits exactly the brute-force set, at any cell size.
- **Dedup no longer has a magic number.** The key is `(uint64_t(a) << 32) | b`, and `a < b` always holds because indices are inserted in ascending order per cell. It is exact for any body count, and it doesn't depend on cell size.
- **Output order is deterministic.** `hits` is sorted by index pair, so the grid returns pairs in the same order as brute force. The comparison no longer depends on `unordered_map` iteration order.
- **Large coordinates are guarded.** `toCell` clamps to ±1e8, which also catches NaN. A body that flies far away or has a bad position no longer causes undefined behaviour in the float-to-int cast. `cellSize <= 0` returns an empty list.
- **Comparison method is sound.** `makePairSet` builds a `std::set` of `std::minmax` pairs, so `(A,B)` and `(B,A)` compare equal. It replaces the earlier `ptr * 1000000` key that could collide.

### Evidence in the scene ([BroadPhaseComparisonScene.cpp](../../src/scenes/BroadPhaseComparisonScene.cpp))
The scene now has two layouts, switched by a radio button, and it re-runs both algorithms every step on the same bodies.
- **Test layout** is a deliberate set of tricky cases:
  - the original non-overlapping circles that share a cell at `cellSize = 2` (the old bug),
  - a body far from everything,
  - overlapping static circles in negative coordinates,
  - a dynamic body overlapping one of those statics,
  - a large rotated box that spans many cells at small cell sizes, so the same pair is reachable through several cells,
  - two boxes with exactly touching AABBs,
  - circles straddling the origin.
  
  These are well chosen. Each one targets a specific failure of the earlier code or of a typical grid.
- **Sparse layout** (criterion 2) spawns 50–1000 bodies (default 300) in an 80×50 world with a fixed seed, so runs are comparable. They are mixed circles and boxes, about 10% static, drifting and bouncing off the world bounds.
- **Counters** (criterion 2): `BroadPhaseStats` is an optional out-parameter on both functions. It records `overlapTests` for both algorithms, plus `cellPairVisits` and `cellInsertions` for the grid. Brute force counts one test per non-static-static pair. The grid counts one test per distinct pair that shares a cell. The scene shows pair counts, test counts, the percentage and the "Nx fewer" ratio. For 300 bodies brute force does roughly 44,000 tests, while the grid should do only a small fraction of that at a cell size near the body size. Please confirm the exact figure on screen.
  - The honest framing matters here, and you handle it well: the grid also shows `cellInsertions` and `cellPairVisits`, so the saving isn't presented as free. See the first note below on which counter best matches "pair-vs-pair comparisons."
- **Cell-size independence** (criterion 3): the "Pair sets match" line is live, and the cell-size slider (0.25–20) feeds the same comparison every step. On top of that, a sweep checks 13 sizes from 0.1 to 200 on the current bodies and reports `n/13 sizes match`, with the first mismatching size if any. That covers sizes far smaller and far larger than the bodies, which is exactly the range where dedup or filter mistakes would show.

### Structure
- **The earlier refactor notes were taken.**
  - Cell-range computation is one helper, `computeCellRange`, shared by insertion and by `findOccupiedCells`.
  - `drawSpatialGrid` moved to [BodyDrawing.h](../../src/scenes/BodyDrawing.h), which now draws from `findOccupiedCells`. It no longer duplicates the grid loop or redefines `IntVec2`.
  - `BroadPhase.h` no longer includes `Renderer.h`, so physics headers don't pull in graphics. Searching `src/physics` finds no renderer dependency.
- **The hash is better.** `IntVec2Hash` now uses the prime multipliers, with unsigned arithmetic so negative cells wrap rather than overflow.

### Notes (none blocking)
1. **Which counter is "comparisons"?** `overlapTests` counts distinct pairs tested, after the dedup set has already filtered repeats. The dedup itself costs one hash lookup per `cellPairVisits`, so the grid's real per-pair work is closer to `cellPairVisits` than to `overlapTests`. Both are shown, which is fine. Say so in a comment, or put the ratio on `cellPairVisits` as well. For sparse bodies the two are nearly equal, so the conclusion doesn't change.
2. **The sweep runs every fixed step.** Each step does 14 grid builds plus two brute-force passes (`world.step` already runs brute force, and the scene runs it again). At 1000 bodies and a cell size of 0.1, one body can touch dozens of cells. This is probably fine at the default 300, but I haven't measured it. If the scene stutters, run the sweep only on change (bodies, count or layout) or every N frames, and reuse `world.getCandidatePairs()` instead of recomputing brute force.
3. **A very large AABB with a tiny cell size is still expensive.** The clamp stops undefined behaviour, but a box thousands of cells wide still loops over all of them. This is a known limit of uniform grids, and the task's hint on Exercise 12 covers it. It is not a bug.
4. **`PhysicsWorld::step()` still uses brute force.** That matches the task, whose criteria are scene-level. A world-level `BroadPhaseMode` setting would let later exercises switch between the two on live data.
5. **`onEnter()` is called from inside `onRender` when the layout changes.** It works, because the world is not drawn again after that point in the frame, but it is easy to break later. Setting a "rebuild" flag and handling it at the start of `onUpdate` is safer.

## Carry-over notes
- The Exercise 5 note about unbounded `rotation` stays resolved: `integrate` wraps with `std::remainder` into [-π, π].
- `imgui.ini` is still tracked and shows up as a diff in nearly every commit. Consider adding it to `.gitignore`.
- `AABBScene`'s dead `checkOverlap` (see Task 6-1) is the only item left from the first pass.

## Wrap-up
Exercise 6 is complete. Before Exercise 7, optionally finish `checkOverlap` in `AABBScene`. Exercise 7 (narrow phase) consumes `BodyPair` from `findPairsBruteForce` or `findPairsSpatialGrid`. Both now return identical, deterministically ordered pair lists, so either works as the input.

Ready to move on to Exercise 7.
