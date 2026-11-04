# Evaluation — Exercise 6

**Verdict: PARTIAL.** Tasks 6-1 and 6-2 pass. Task 6-3 is implemented, but it fails its own "sets must match" criterion on the default scene, and two of its three acceptance criteria have no evidence yet. This was a code and scene review. This shell has no C++ compiler, so I did not build or run the app. A `build/bin` binary exists and is newer than `BroadPhase.cpp`, so the code probably compiles.

| Task | Result |
|---|---|
| 6-1 AABB Computation | PASS (one dead check to fix) |
| 6-2 Brute-Force Broad Phase | PASS |
| 6-3 Spatial Grid Broad Phase | NOT YET: one real bug, two criteria unmet |

## Task 6-1 — AABB Computation
- `AABB` in [AABB.h](../../src/physics/AABB.h) has `min`, `max`, `overlaps`, plus handy `center()` and `size()`. `overlaps` is the standard four-way separation test. It counts touching edges as overlapping, and the comment explains why: broad phase must never miss a pair, and narrow phase rejects false positives. That gives you "two circles just touching produce touching AABBs."
- `computeAABB` in [RigidBody.cpp](../../src/physics/RigidBody.cpp) is correct for both shapes. The circle is `position ± radius`. The box transforms all four corners through `Transform::localToWorld`, then takes min/max. It seeds from the first corner rather than zero, so a body far from the origin is handled correctly. It is recomputed on demand, which the task allows.
- [AABBScene.cpp](../../src/scenes/AABBScene.cpp) covers the visual criteria well:
  - five static boxes at 0, π/8, π/4, 3π/8 and π/2, each with its AABB drawn,
  - a spinning box whose AABB should pulse,
  - three circle pairs at gaps of exactly touching, just apart and just overlapping, coloured red or green by `overlaps`.
- **Issue: the "known cases" check is dead code.** `checkOverlap` computes `actual` and throws it away. It ignores `label` and `expected`, and prints and asserts nothing. The same goes for `s0` and `s45` at the end of `onEnter`: computed, never shown. So the five `overlaps` cases (overlapping, separated on x, separated on y, touching, fully inside) are never checked anywhere. I read the logic by hand and all five come out right, but the scene doesn't show it. Fix: have `checkOverlap` store `actual == expected` and show a "5/5 overlap cases pass" line in ImGui, and show `s0` and `s45` as text so the size difference is a number.

## Task 6-2 — Brute-Force Broad Phase
- `findPairsBruteForce` in [BroadPhase.cpp](../../src/physics/BroadPhase.cpp) is a correct `O(n²)` loop. Starting `j` at `i + 1` rules out self-pairs and `(A,B)`/`(B,A)` duplicates by construction. It skips static-static pairs. It computes each AABB once up front instead of once per pair, which is a good call.
- `PhysicsWorld::step()` runs it after integration and stores the result in `candidatePairs`, with a `getCandidatePairs()` accessor. `removeRigidBody` also purges any pair that references the removed body, so no dangling pointers. The last two Exercise 5 evaluations flagged pointer lifetime, and you carried the lesson over.
- [BroadPhaseScene.cpp](../../src/scenes/BroadPhaseScene.cpp) is a thorough demonstration:
  - two groups drift through each other, so the pair count rises, then falls,
  - a chain of three circles where only neighbours touch (expect 2 pairs),
  - isolated bodies that must never pair,
  - overlapping static-static boxes that must be skipped,
  - a static-dynamic overlap that must be kept,
  - a live "Pair list valid" readout that checks for self-pairs, static-static pairs and duplicates.
- Minor: the "expect 2 pairs" cluster only works because the circles touch at exactly one coordinate and `overlaps` is inclusive. That is fine, but a comment saying so would stop it looking like a magic number.

## Task 6-3 — Spatial Grid Broad Phase

What is right:
- The structure matches the task. It uses an `unordered_map<IntVec2, vector<size_t>>`, with `IntVec2` and its hash in [IntVec2.h](../../src/math/IntVec2.h). Each body goes into every cell its AABB spans, using `floor(coord / cellSize)` for min and max. Negative coordinates are handled correctly because it uses `floor`, not integer truncation.
- It cannot miss a pair. Two overlapping AABBs overlap in a region, so some cell is inside both ranges, and both bodies land in it. The grid is never tighter than brute force.
- Cross-cell dedup works through a `seen` set keyed on the ordered index pair. Static-static pairs are skipped.
- Cell size is a parameter with a live slider in the scene. `drawSpatialGrid` shows occupied cells.

What is wrong or missing:

1. **Bug: the grid never tests AABB overlap, so its pair set is a superset of brute force.** Bodies that merely share a cell are emitted as pairs. The task says the grid "can't be looser in coverage." Your own default scene shows the problem. `BroadPhaseComparisonScene` has circle A at (0,0), r=1, so AABB x is [-1, 1]. Circle B is at (2.5,0), r=1, so x is [1.5, 3.5]. They don't overlap, and brute force doesn't pair them. With `cellSize = 2` they share cell 0, so the grid does pair them. The scene should print `Brute-force: 0 pairs, Spatial grid: 1 pairs, Sets match: NO`. It is also why criterion 3 matters: with the filter missing, the output *does* depend on cell size. Small cells hide the bug, large cells expose it.
   **Fix:** in the inner loop, after the static check and before the dedup, add `if (!boxes[idxA].overlaps(boxes[idxB])) continue;`. This needs the AABBs cached in a vector during insertion, as `findPairsBruteForce` does. That one line makes the sets match at every cell size.

2. **Criterion 1 ("sets exactly match") is not demonstrated.** Because of bug 1, the check fails on the default scene. The test also has weaknesses:
   - It runs once in `onEnter` and prints to the console. It does not run again when the cell-size slider moves, and nothing shows in the UI.
   - The scene is five fixed bodies. It is the only comparison scene, so it isn't a real test.
   - `pairSet` builds its key as `ptrA * 1000000 + ptrB`. This is not unique: the multiplication overflows `uintptr_t`, and different pairs can map to the same key, giving false matches. `BroadPhaseScene::pairsAreValid` already does it right with `std::set<std::pair<const RigidBody*, const RigidBody*>>` and `std::minmax`. Reuse that. The same issue exists in `findPairsSpatialGrid`, where `min * 1000000 + max` is only safe below a million bodies. A `std::set` or `unordered_set` keyed on a `pair`, or sorting and `unique`, removes the magic number.

3. **Criterion 2 (fewer comparisons) is not met.** There is no comparison counter anywhere, and no scene with a few hundred sparse bodies. This is the actual point of the task, so it needs to be built. Suggestions:
   - Add a `size_t *comparisons` out-parameter, or a small stats struct, to both functions. Brute force counts every `boxes[i].overlaps(boxes[j])` call. The grid counts every candidate pair examined in the cell loops, before dedup.
   - Add a scene that spawns about 300 randomly placed bodies over a large world, and show brute count, grid count and the ratio in ImGui.

4. **Criterion 3 (cell size changes keep results correct) is not demonstrated.** The slider exists, but nothing re-runs the comparison when it moves, so correctness at different cell sizes is never checked. Once bug 1 is fixed, re-run the comparison every frame, or on slider change, and show "sets match: yes/no" next to the counts. Then drag the slider from 0.5 to 10 and watch it hold.

Smaller notes on this task:
- **`drawSpatialGrid` duplicates the grid-building loop and redefines `IntVec2` and `IntVec2Hash` as local structs.** These shadow the ones in `IntVec2.h`. It compiles, but it is a copy-paste that will drift. Pull the cell-range computation into one helper, for example `cellRange(const AABB&, float cellSize) -> {minCell, maxCell}`, and use it from both the grid and the drawing code. Drawing only needs the *set* of occupied cells, so it doesn't need the body lists at all.
- **A physics module depends on the renderer.** `BroadPhase.h` includes `graphics/Renderer.h` for `drawSpatialGrid`, and `PhysicsWorld.h` includes `BroadPhase.h`. Debug drawing belongs next to `drawBody` and `drawAABB` in `scenes/BodyDrawing.h`. Exercise 5 noted that scenes only see `Renderer`. Keeping that direction one-way means the physics code stays compilable without graphics headers.
- **Hash quality.** `x ^ (y << 1)` collides readily: `(2,0)` and `(0,1)` both hash to 2. That only costs speed and not correctness, since the map uses `==`, but it is cheap to improve with a prime multiplier, e.g. `x * 73856093 ^ y * 19349663`.
- **Non-deterministic pair order.** The grid iterates an `unordered_map`, so pair order differs from brute force and between runs. That is fine for sets. It may matter once Exercise 8 solves contacts in order. Sorting pairs by index, or iterating bodies instead of cells, would make it stable.
- **Large coordinates.** `static_cast<int>(floor(x / cellSize))` is undefined if a body flies far enough away. A very large AABB with a small cell size also loops over millions of cells. This is not needed now, but worth a clamp or an assert before Exercise 12.
- **The grid isn't used by `PhysicsWorld::step()`.** It still calls `findPairsBruteForce`. The task's acceptance criteria are scene-level, so that is fine for now. A world-level setting such as `BroadPhaseMode::{BruteForce, Grid}` would let the scenes and later exercises compare both on live data.

## Carry-over from Exercise 5
- The unbounded `rotation` note is resolved: `integrate` now wraps with `std::remainder` into [-π, π], and the header documents it. That matters for AABBs because the corner transform uses `sin` and `cos` of `rotation`.

## Other
- `imgui.ini` is tracked and shows up as a diff in nearly every commit. Consider adding it to `.gitignore`.

## What to do before Exercise 7
1. Add the `overlaps` filter to the grid inner loop. This is a one-line fix.
2. Add a comparison counter, plus a scene with a few hundred sparse bodies. Show the counts and the ratio.
3. Re-run the set comparison live when the slider changes, with a robust `pair`-keyed set. Show the result in ImGui.
4. Make `checkOverlap` in `AABBScene` report its results.
5. Optionally: share the cell-range helper, and move `drawSpatialGrid` out of the physics module.

Once 1–3 are done, Task 6-3 passes and Exercise 6 is complete.
