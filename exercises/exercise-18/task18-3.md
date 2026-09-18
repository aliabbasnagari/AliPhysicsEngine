# Task 18-3: Build The Demo

**Module:** Exercise 18 — Capstone: God-Level Demo
**Goal:** Build one complete, polished demo scene that genuinely exercises the engine you built across all 18 modules — the actual payoff of this whole curriculum.

## Task
Pick one (or design your own of similar scope):
- **Stacking tower**: a Jenga-like or pyramid stack of boxes, stable at rest, that can be knocked over with an applied impulse and settles believably — exercises Exercises 5-9 hard
- **Newton's cradle**: a row of pendulums (revolute/distance joints, Exercise 10) demonstrating momentum transfer through collisions
- **A small physics-driven game**: e.g. a pool/billiards table (circle-circle collision, friction, restitution tuning) or a marble-run (static geometry, raycasting for triggers, rolling circles)

Whichever you pick, it should combine: rigid bodies with correct mass/inertia,
collision detection and resolution, at least one joint type, and be built
using your Task 18-2 tooling (not hand-coded body-by-body in C++, though a
few lines of setup code calling into your engine is fine too).

## Acceptance Criteria
- [ ] The demo runs stably for an extended period (minutes, not seconds) without exploding, sinking through the floor, or visibly incorrect behavior
- [ ] It's visually clear what's happening (reasonable debug draw, maybe basic color coding) — a reviewer with no context should be able to tell what the demo is doing within a few seconds of watching it
- [ ] It showcases at least 3 distinct engine subsystems working together (e.g. collision + joints + friction, or collision + sleeping + raycasting)
- [ ] You've saved it as a loadable scene file (Task 18-1) so it can be reopened without rebuilding it by hand

## Hints
- This is the module to actually have fun with — you've earned it. Polish (sound is out of scope, but visual feedback, camera framing, a title/instructions overlay) genuinely matters for how satisfying this feels as a capstone.
