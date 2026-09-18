# Task 16-3: SIMD a Hot Path

**Module:** Exercise 16 — Data-Oriented Performance
**Goal:** Use SIMD intrinsics (or a SIMD-friendly library) to process multiple bodies' data per instruction on a genuinely hot, data-parallel loop.

## Concept
SIMD (Single Instruction, Multiple Data) lets one CPU instruction operate
on several values at once (e.g. 4 or 8 floats with SSE/AVX). It pays off
best on tight, branch-light, data-parallel loops over arrays of numbers —
which is exactly what Task 16-2's SoA refactor sets you up for (e.g. AABB
overlap tests, or batch-updating positions from velocities).

## Task
- Using Task 16-1's profiler data, pick one genuinely hot, data-parallel loop (a good candidate: broad-phase AABB overlap testing, or integrating many bodies' positions from velocities)
- Implement a SIMD version using either raw intrinsics (`<immintrin.h>`, SSE or AVX) or a small header-only SIMD wrapper library if you'd rather not hand-write intrinsics — either is a legitimate choice, the value here is in the batching approach, not necessarily raw intrinsic fluency
- Keep the scalar version around behind a compile-time or runtime toggle so you can A/B them
- Benchmark scalar vs SIMD on the same stress-test scene from Task 16-1, and verify the SIMD version produces *identical* (or numerically negligible-difference) results to the scalar version — a fast wrong answer is worse than a correct slow one

## Acceptance Criteria
- [ ] You have a working SIMD version of at least one real hot path, with matching output to the scalar version on your test scene
- [ ] You have actual before/after timing numbers for that specific loop
- [ ] You understand and can state why this particular loop was (or wasn't) a good SIMD candidate — branch-heavy, pointer-chasing code (e.g. tree traversal) generally isn't; tight numeric array loops generally are

## Hints
- Don't chase SIMD-ing something like GJK/EPA or tree traversal here — those are branchy and irregular, poor SIMD candidates. Pick the most boring, regular, numeric loop you have; that's exactly where SIMD earns its keep.
