# Evaluation — Exercises 1–3

**Verdict: PASS.** All acceptance criteria for Exercises 1–3 are met. Code is clean, correctly commented (explains *why*, not *what*), and the architecture holds up well for later exercises.

## Exercise 1 — Math Foundations
- `Vec2`, `Vec3`, `Mat4`, `Quaternion` all correct against every acceptance criterion (cross product, dot, perp, translation/rotation composition, quaternion↔matrix equivalence).
- Conventions (column-major `Mat4`, quaternion multiply order, right-hand rule) are documented as the task required.
- Nit: `Vec3::perp()` is a leftover copy-paste from `Vec2` (ignores `z`, not in the spec, unused) — delete it.
- Nit: `src/tests/exercise1.cpp` defines `runTest11`–`runTest14` to check the acceptance criteria, but `main.cpp` only `#include`s the `.cpp` — none of the test functions are ever called, so they never actually run. Either call them once at startup or delete the file; right now it's dead code that looks like verification but isn't.

## Exercise 2 — Application Shell & Render Loop
- `Application::run()` implements the accumulator pattern correctly: frame dt clamped to 0.25s, drained in fixed 1/60s steps, one render per frame regardless of update count.
- `Renderer` wraps `ImGui`'s background draw list correctly for line/circle/box; world→screen convention is documented in `Renderer.h`.
- `main.cpp` is a two-line entry point; `Application` fully owns GLFW/glad/ImGui lifecycle, matching the task exactly.
- Beyond spec (in a good way): a `Scene` interface + per-exercise scenes + in-app menu, instead of a single sandbox. This makes every later exercise self-contained and swappable at runtime rather than hand-editing one scratch file each time.

## Exercise 3 — Particles & Integrators
- `Particle`: explicit and semi-implicit Euler both correct and correctly ordered; `inverseMass == 0` correctly pins static particles.
- `VerletParticle`: correct Störmer-Verlet update, `oldPosition` seeded correctly from initial velocity, velocity reconstructed as `(position - oldPosition) / dt`.
- Drag: linear and quadratic modes both correct — quadratic uses `velocity * speed`, avoiding the `velocity * velocity` trap the task specifically warns about.
- Force accumulator pattern: the `IForceReceiver` interface lets one `ForceGenerator` (`Gravity`/`Drag`/`Wind`/`AnchoredSpring`) act on both `Particle` and `VerletParticle` uniformly. `WindGenerator` + `ForceAccumulatorScene` genuinely prove the "new force costs zero changes to `Particle`/`PhysicsWorld`" criterion, not just in theory.
- Five scenes (gravity, Euler comparison w/ energy plot, Verlet comparison w/ position/velocity error plot, drag, force-accumulator toggle) go well past "make it visible" — this is real comparison tooling, and the spring used for the Euler comparison (`AnchoredSpringGenerator`) is a good early start on Exercise 4.

## Action items
1. Delete or wire up the dead test functions in `src/tests/exercise1.cpp`.
2. Delete the stray `Vec3::perp()`.

Everything else is ready to build Exercise 4 on top of.
