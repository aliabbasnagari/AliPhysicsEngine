# Task 16-1: Profile Your Physics Step

**Module:** Exercise 16 — Data-Oriented Performance
**Goal:** Measure where your physics step actually spends time, before optimizing anything — the prerequisite for every other task in this module.

## Concept
Optimizing without profiling is guessing. You almost certainly have
intuitions about what's slow (GJK? the solver? broad phase?) — this task's
whole point is finding out whether those intuitions are right, using actual
timing data from your own engine on your own test scenes.

## Task
- Add simple scoped timers (e.g. an RAII `ScopedTimer` that records elapsed time into a named bucket on destruction) around the major phases of `PhysicsWorld::step()`: broad phase, narrow phase, solving, integration
- Display the results as an ImGui overlay (reusing your Exercise 2 debug draw/ImGui setup): per-phase time in ms, and total step time, updated live
- Build (or reuse) a stress-test scene with a large number of bodies (start with whatever makes your current engine visibly struggle — a few hundred to a couple thousand, depending on what you've built through Exercise 9-15) and observe the breakdown

## Acceptance Criteria
- [ ] You have a live, on-screen breakdown of time spent per physics phase, updated every frame
- [ ] You can identify, with actual numbers (not a guess), which phase dominates your current stress-test scene's frame time
- [ ] The profiler overlay itself has negligible overhead (verify: toggling it on/off shouldn't meaningfully change measured step time)

## Hints
- Resist the urge to fix anything you notice while building this task — write down what you find, then let Tasks 16-2/16-3/16-4 (and your own judgment about what's actually worth optimizing) address it deliberately, informed by these numbers rather than instinct.
