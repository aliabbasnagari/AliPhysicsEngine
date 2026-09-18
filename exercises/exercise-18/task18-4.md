# Task 18-4: Stress Test & Final Benchmark Report

**Module:** Exercise 18 — Capstone: God-Level Demo
**Goal:** Find and document your engine's actual limits — a properly measured, honest report, capping off Exercise 16-17's measurement discipline.

## Task
- Build a stress-test scene generator: spawn an increasing number of bodies (e.g. 100, 500, 1000, 2000, 5000...) in a reasonably realistic configuration (not degenerate — e.g. not all stacked in exactly one pixel), using your Task 16-1 profiler to record step time at each count
- Find the body count where you drop below a target frame budget (e.g. 16.6ms for 60Hz) with your default settings, and separately with your parallel solver (Exercise 17) enabled if you built it
- Note where time is actually going at your breaking point (broad phase? narrow phase? solver iterations? — Task 16-1's breakdown should answer this directly)
- Write up the results in `exercises/exercise-18/benchmark-report.md` (or similar, your own file — not required reading for anyone else, but a real artifact you produced): body counts tested, timings, bottleneck identified at scale, and what you'd optimize next if you kept going

## Acceptance Criteria
- [ ] You have real measured numbers (not estimates) for how your engine scales with body count
- [ ] You've identified your actual current bottleneck at scale, backed by profiler data, not assumption
- [ ] The report names concrete next steps (informed by everything from Exercise 12 through 17) rather than vague "make it faster" — e.g. "broad phase is fine, narrow-phase GJK calls dominate past 2000 bodies because most are sphere-sphere but still routing through the general path"
- [ ] You've compared at least single-threaded vs parallel-solver (if built) numbers side by side

## Hints
- This report is the natural end of the curriculum — if you've gotten here with working code and real numbers to back up your understanding at every stage, you've built a real physics engine from scratch and genuinely understand how it works, not just that it works. That's the actual goal this whole curriculum has been aimed at.
