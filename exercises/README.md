# AliPhysicsEngine — Learning Curriculum

A from-scratch physics engine, broken into exercises that take you from
"what is a vector" to "GJK/EPA, XPBD soft bodies, and a deterministic
lockstep simulation." Beginner to god-tier, eighteen modules.

## How this works

- Each `exercise-N/` folder is one module. Each `taskN-M.md` inside it is one
  focused implementation task.
- Tasks are **briefs, not tutorials**: a goal, the key concept/formula, what
  to build, and acceptance criteria you can check yourself against. No
  solution code - you write every line.
- Work through modules in order. Later modules assume earlier ones exist and
  work (e.g. Module 8's collision resolution assumes Module 7's collision
  detection is producing correct contacts).
- Implement each task directly in the relevant `src/` file(s) named at the
  top of the task. When you create a new `.cpp` file, add it to
  `add_executable(AliPhysicsEngine ...)` in the root `CMakeLists.txt`.
- Build/run with `make run` (or `cmake --build build && ./build/bin/AliPhysicsEngine`)
  after each task. If it builds and the acceptance criteria hold, move on.
- Use `src/sandbox/Sandbox.h/.cpp` as your scratch scene for trying things
  out visually - most tasks expect you to render/inspect the result there.

## Roadmap

### Tier 0 — Foundations
1. [Math Foundations](exercise-1/README.md) — Vec2, Vec3, Mat4, Quaternion
2. [Application Shell & Render Loop](exercise-2/README.md) — fixed timestep loop, debug draw, Application class

### Tier 1 — Particle Physics
3. [Particles & Integrators](exercise-3/README.md) — Euler, semi-implicit Euler, Verlet, force accumulators
4. [Springs & Particle Constraints](exercise-4/README.md) — Hooke's law, distance constraints, rope, cloth grid

### Tier 2 — 2D Rigid Bodies
5. [Rigid Body Fundamentals](exercise-5/README.md) — mass, inertia, torque, PhysicsWorld
6. [Broad-Phase Collision Detection](exercise-6/README.md) — AABBs, brute force, spatial grid
7. [Narrow-Phase Collision Detection](exercise-7/README.md) — circle/polygon, SAT, manifolds
8. [Collision Resolution](exercise-8/README.md) — impulses, restitution, friction
9. [Stable Contacts & Solver Quality](exercise-9/README.md) — persistent manifolds, warm starting, sleeping
10. [Joints & Constraints](exercise-10/README.md) — distance, revolute, prismatic, motor joints

### Tier 3 — Scaling Up 2D
11. [Continuous Collision Detection & Raycasting](exercise-11/README.md) — ray casts, tunneling, swept shapes
12. [Spatial Partitioning at Scale](exercise-12/README.md) — sweep and prune, dynamic AABB tree (BVH)

### Tier 4 — Into 3D
13. [3D Math & Rigid Bodies](exercise-13/README.md) — inertia tensors, quaternion integration
14. [3D Collision Detection](exercise-14/README.md) — GJK, EPA, 3D contact generation

### Tier 5 — Soft Bodies
15. [Position-Based & Extended Position-Based Dynamics](exercise-15/README.md) — PBD, XPBD, cloth, soft blobs

### Tier 6 — God Tier: Performance & Architecture
16. [Data-Oriented Performance](exercise-16/README.md) — profiling, SoA layout, SIMD, allocators
17. [Multithreading & Determinism](exercise-17/README.md) — parallel solving, fixed-point determinism, lockstep

### Tier 7 — Capstone
18. [Capstone: God-Level Demo](exercise-18/README.md) — serialization, sandbox tooling, a real demo, stress test

## Recommended reading (optional, but this is where the real understanding comes from)

- Erin Catto, *Box2D Lite* source + GDC slides ("Fast and Simple Physics using Sequential Impulses", "Soft Constraints")
- Christer Ericson, *Real-Time Collision Detection* (the book on narrow-phase and broad-phase)
- Ian Millington, *Game Physics Engine Development*
- Randy Gaul's blog (impulse solvers, GJK/EPA write-ups, qu3e source)
- Dirk Gregorius, "The Separating Axis Test" and "Robust Contact Creation" (GDC)
- Müller et al., *Position Based Dynamics* and *XPBD: Position-Based Simulation of Compliant Constrained Dynamics* papers
- Casey Muratori / Kevin Falk explainers on GJK/EPA if the papers are too dense at first

## Progress tracking

There's no automated checklist here on purpose - track your own progress
however you like (a TODO.md, GitHub issues, checkboxes in your fork of this
README, whatever sticks). The acceptance criteria in each task are your
source of truth for "done."
