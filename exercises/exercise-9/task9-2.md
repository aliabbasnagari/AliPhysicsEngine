# Task 9-2: Warm Starting

**Module:** Exercise 9 — Stable Contacts & Solver Quality
**Goal:** Use last frame's accumulated impulse as this frame's starting guess, dramatically improving stacking stability and convergence speed.

## Concept
Instead of starting each contact's impulse from zero every frame (and
re-deriving the "right" impulse via several solver iterations), apply last
frame's *converged* impulse immediately at the start of this frame, then
let the solver iterations only correct the small *delta* needed. This is
the single biggest stability win in a sequential-impulse solver — it's why
real-time engines can resolve stacks of dozens of boxes with only a handful
of iterations.

## Task
- On each persistent contact point (Task 9-1), store the accumulated normal and tangent impulse magnitudes from the previous frame
- At the start of each step, before running solver iterations, apply each contact's *stored* impulse from last frame immediately (same apply logic as Task 8-1/8-4, just using the cached magnitude instead of a freshly computed one)
- During the solver iterations that follow, instead of computing impulse from scratch each time, compute the *change* needed and clamp the *running total* (e.g. accumulated normal impulse must stay >= 0 — never a "pulling" impulse) — this is a standard refinement called **accumulated impulse clamping**
- Store the final accumulated impulse per point at the end of the step, for next frame

## Acceptance Criteria
- [ ] A stack of 4-5 boxes settles into a stable, non-jittering pile noticeably faster/more solidly than before this task
- [ ] Removing warm starting (temporarily zero the carried-forward impulse) visibly degrades stack stability on the same scene — confirms it's actually doing something
- [ ] Accumulated normal impulse never goes negative (would mean the solver is pulling bodies together, which is physically wrong for a contact)

## Hints
- Keep Exercise 8's "compute from scratch" impulse formulas nearby for comparison — warm starting changes the bookkeeping (what you start from, what you clamp), not the underlying impulse math.
