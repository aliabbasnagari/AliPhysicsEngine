# Evaluation — Exercise 4

**Verdict: PASS.** All acceptance criteria for Exercise 4 are met, including the optional mouse-drag stretch goal on the cloth.

## Task 4-1 — Hooke's Law Spring
- `SpringForceGenerator` is correct: `F = -k * extension` along the normalized spring axis, damping projects relative velocity onto that axis and opposes it, equal-and-opposite applied to both endpoints via Newton's third law.
- `HookeSpringScene` demonstrates all three criteria directly: damping settles the free particle, `damping = 0` keeps energy bounded under semi-implicit Euler, and the live `omega*dt` readout (`sqrt(k/m) * dt`) correctly predicts when explicit Euler is unconditionally unstable and when semi-implicit will blow up. This is a genuinely good way to make the stiffness/timestep tradeoff tangible instead of just asserting it.

## Task 4-2 — Distance Constraint
- `solveDistanceConstraint` in `DistanceConstraint.h` is correct: separation error, direction, split by inverse-mass share (`invMass / totalInvMass`), applied after integration (not as a force) — matches the task's spec exactly.
- Templating it over `ParticleT` (used for both `Particle` and `VerletParticle`) is a reasonable call: the two types share the exact public shape (`position`, `inverseMass`) the solver needs, so this avoids duplicating the same ~15 lines for the rope later. Not over-engineered — it's one template, not a constraint hierarchy.
- `DistanceConstraintScene` proves all three criteria live: a constraint-vs-spring pendulum comparison (constraint length stays flat, spring length oscillates), an anchor-drift readout showing the `inverseMass == 0` particle never moves, and a free-free pair in a gravity-less world isolating the 50/50 split with a numeric diff.

## Task 4-3 — Rope
- `RopeScene` chains `VerletParticle`s with `VerletDistanceConstraint`s, anchors particle 0 via `mass = 0`, and runs the iteration loop through `PhysicsWorld::setConstraintIterations` — exactly the structure the task describes (integrate once, relax N times).
- Iteration count and segment length are both live sliders, and the scene reports actual vs. rest total length with a stretch percentage, which is the right way to make "more iterations = less stretch" an observable number instead of just a visual impression.
- Dragging the anchor and perturbing the free end are both exposed, so the propagation-along-the-chain criterion is directly testable.
- Small addition not asked for but sensible: a touch of linear drag so the (nearly energy-conserving) Verlet rope settles instead of swinging forever — the reasoning is explained in a comment rather than silently changing behavior.

## Task 4-4 — Cloth Grid
- `ClothScene` builds an `M x N` `VerletParticle` grid, links horizontal/vertical neighbors via the same `VerletDistanceConstraint`, pins row 0 (either both corners or the whole row, toggle-able), and renders it as a wireframe with per-link strain coloring — all per spec.
- Shear (diagonal) constraints are implemented as the stretch goal and added *after* structural links so they solve last in each Gauss-Seidel pass, which is the right ordering call (structure takes priority, shear just resists collapse).
- Mouse drag (also a stretch goal) is fully implemented: picks the nearest unpinned particle, pins it to infinite mass while held, clamps drag speed so a fast flick doesn't teleport it, and resets `oldPosition` on release to avoid injecting spurious velocity. This is more than "optional but recommended" called for, and it's done carefully (no stale-mass or stale-velocity bugs).
- Average/worst-link strain readout gives a concrete number for "doesn't explode or oscillate wildly," rather than relying on eyeballing.

## Minor notes
- None blocking. The `DistanceConstraintT` template, `IForceReceiver`-based force generators, and the `Particle`/`VerletParticle` split from Exercise 3 all carried forward into this exercise cleanly with no rework needed — a good sign the Exercise 3 architecture was built with the right seams.

Ready to move on to Exercise 5 (Rigid Body Fundamentals).
