# Task 17-1: Parallel Broad-Phase

**Module:** Exercise 17 — Multithreading & Determinism
**Goal:** Spread broad-phase pair generation (Exercise 6/12) across multiple threads.

## Concept
Broad-phase pair generation is a natural first target for parallelism:
different regions of a spatial structure (grid cells, BVH subtrees, SAP
segments) can largely be processed independently, and it doesn't (yet)
touch shared, order-sensitive state the way the solver does (that's why
Task 17-2, not this task, has to deal with islands).

## Task
- Add a minimal thread pool (or use `std::async`/`std::for_each` with a parallel execution policy if your standard library supports it — either is fine, the concept matters more than the specific mechanism)
- Parallelize your Exercise 12 broad phase (grid, SAP, or BVH — whichever you kept as primary): split work by region/subtree/chunk across threads, merge the resulting candidate pair lists
- Ensure merging is race-free (each thread writes to its own local pair list, merged single-threaded afterward, is the simplest correct approach — avoid a shared, lock-protected list unless you have a specific reason to)
- Benchmark against the single-threaded version (reuse Task 16-1's profiler) on your large stress-test scene

## Acceptance Criteria
- [ ] Candidate pair output is identical to the single-threaded version (set equality — order may differ) on the same scene
- [ ] Measured speedup on a multi-core machine for a large scene (don't expect a clean Nx speedup for N cores — some speedup, correctly measured, is the goal)
- [ ] No data races (if you have access to a thread sanitizer, run it; otherwise, stress-test extensively and reason carefully about what each thread touches)

## Hints
- Small scenes may get *slower* under parallelization (thread overhead exceeds the work saved) — measure across a range of body counts and note where the crossover point is, rather than assuming "parallel is always better."
