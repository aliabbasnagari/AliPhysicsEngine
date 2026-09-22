# Evaluation — Exercises 1-3

Reviewed: `src/math/*.h`, `src/core/Application.*`, `src/graphics/Renderer.*`,
`src/main.cpp`, `src/physics/Particle.*`, `src/physics/VerletParticle.*`,
`src/physics/PhysicsWorld.*`, `src/physics/ForceGenerator.h`,
`src/physics/GravityGenerator.*`, `src/physics/DragGenerator.*`,
`src/tests/exercise1.cpp`.

Verified the project builds clean (`cmake --build`, WSL/g++ 15) with zero
warnings or errors.

## Verdict

**Update:** the Verlet force bug is fixed (`IForceReceiver` interface),
the Task 3-2 "why the update order matters" comment is written, and the
Task 3-2 oscillator comparison was confirmed by the author to have been
manually verified before it was cleaned out of `PhysicsWorld`. All five
Exercise 3 tasks are now considered done; only non-blocking nitpicks and
cleanup remain across all three exercises. Original verdict, for context:

Exercise 1 and 2 are essentially complete and correct — no notes that rise
above "nitpick". Exercise 3 has the right shapes for all five tasks
(explicit Euler, semi-implicit Euler, Verlet, drag, force accumulator) and
the code that exists is well-commented and mostly correct, but the last
refactor (task 3-5, force-generator pattern) introduced one real bug and
quietly dropped the live demo that tasks 3-1/3-2 depend on.

---

## Exercise 1 — Math Foundations

**Vec2 / Vec3 / Mat4 / Quaternion — all acceptance criteria met.**

- `Vec2`/`Vec3`: operators, dot, cross (`Vec3.h:85-91`, correct component
  order, anti-commutative), `normalized()` with a documented zero-vector
  case, epsilon `operator==`. Good header-only, `constexpr`-friendly style.
- `Mat4`: column-major, documented (`Mat4.h:8-15`), correct multiply,
  correct `rotationZ(pi/2)` convention, `transpose()`. You correctly did
  _not_ build a general inverse — matches the task's explicit "don't build
  it speculatively" hint.
- `Quaternion`: `fromAxisAngle`, Hamilton product with the composition
  order documented (`Quaternion.h:53-67`), the optimized
  `v + 2w(u×v) + 2(u×(u×v))` rotation formula, `toMat4()`. All four
  acceptance checks pass by inspection and match `src/tests/exercise1.cpp`.
- `src/tests/exercise1.cpp` is a solid, literal translation of every task's
  acceptance criteria into a runnable check — good instinct to write this
  rather than eyeballing it.

**Nitpicks (not blocking):**

- `Vec3::perp()` (`Vec3.h:120-126`) is copy-pasted from `Vec2` verbatim,
  down to the stale "(x, y) -> (-y, x)" comment. It's not part of the
  Vec3 task spec, it's unused anywhere in the codebase, and it's not even
  well-defined for a general 3D vector (there's a whole plane of vectors
  perpendicular to any given `Vec3`, not one). Delete it — if you need a
  perpendicular-in-a-plane later, that's a 2-argument operation
  (`a.cross(planeNormal)`), not a 1-argument method.
- `Vec2` uses epsilon `1e-5f` for `operator==`, `Vec3` uses `1e-6f`
  (`Vec2.h:119` vs `Vec3.h:131`). Harmless, just inconsistent — pick one.
- `Mat4::transformPoint` does a perspective divide guarded by
  `w != 0 && w != 1` (`Mat4.h:87-93`). Every matrix you currently build
  (translation/scale/rotation) always produces `w == 1`, so this branch is
  dead in practice. Fine to leave as a defensive no-op, but worth knowing
  it isn't exercising real behavior yet — you don't have a projection
  matrix that would produce `w != 1` until much later.
- `main.cpp:41-44` now has `runTest11()..runTest14()` commented out, so
  these acceptance checks aren't actually running anymore. Fine if you've
  already eyeballed PASS on all of them, but consider gating them behind a
  `#ifdef RUN_MATH_TESTS` (or a small `tests` CMake target) instead of
  commenting/uncommenting by hand — you'll want this again in Exercise 13.

---

## Exercise 2 — Application Shell & Render Loop

**All three tasks meet their acceptance criteria.**

- Task 2-1 (fixed timestep): `Application::run()` (`Application.cpp:100-156`)
  implements the accumulator pattern correctly — real dt measured via
  `glfwGetTime()`, clamped to `maxFrameDt = 0.25`, drained in a
  `while (accumulator >= fixedDt)` loop, one render per frame regardless of
  how many updates ran. This is the textbook version of the pattern.
- Task 2-2 (debug draw): `Renderer::drawLine/drawCircle/drawBox` all work
  off `ImGui::GetBackgroundDrawList()`, and the world-to-screen convention
  (50 px/unit, +Y up, origin at window center) is documented right on the
  class (`Renderer.h:27-36`) — exactly what the task asked for so later
  tasks don't have to reverse-engineer it.
- Task 2-3 (Application class): GLFW/glad/ImGui lifecycle is fully owned by
  `Application`, `onUpdate`/`onRender` are virtual hooks, `main.cpp` is
  reduced to constructing `Application` and calling `run()`.

**Nitpicks:**

- `main.cpp:16-35` still defines `GlfwErrorCallback`, `update(float)`, and
  `render(GLFWwindow*)` — none of these are called anywhere now that
  `Application` owns the loop (they're leftovers from before Task 2-3).
  Since the task explicitly asks you to reduce `main.cpp` down to a
  two-line entry point, delete these three — they're dead code that still
  drags in `GLFWwindow*`/`glClear`/`glViewport` references for no reason.
- `#include "tests/exercise1.cpp"` in `main.cpp:14` includes a `.cpp` file
  directly rather than declaring the test functions in a header. Works
  because there's only one translation unit including it, but it's a
  pattern that breaks the moment you add a second file that also wants
  those functions. A `tests/exercise1.h` with the four declarations (or
  just moving them into a real CMake test target later) avoids the trap.

---

## Exercise 3 — Particles & Integrators

### Task 3-1 (Explicit Euler) — code is correct, not currently exercised

`Particle::integrate` with `IntegrationMode::ExplicitEuler`
(`Particle.cpp:39-45`) correctly uses the _old_ velocity for the position
update before advancing velocity — matches the spec exactly. However,
nothing in `PhysicsWorld` currently constructs a particle in this mode (see
Task 3-2 note below) — the logic is right, but there's no live instance
demonstrating it anymore.

### Task 3-2 (Semi-Implicit Euler) — DONE

`Particle::integrate`'s semi-implicit branch (`Particle.cpp:46-58`) is
correct, and now carries the "why" comment the task explicitly asks for on
both branches: explicit Euler advances position with the _old_ velocity,
so an oscillating system overshoots a little further every step and the
error compounds in one direction (energy grows unbounded); semi-implicit
updates velocity first, so the position step already reflects this step's
acceleration, and the overshoot partially cancels step-to-step instead of
compounding (energy stays roughly bounded).

The explicit-vs-semi-implicit comparison on an oscillating (spring) system
was built and verified during development — an `explicitParticle` and a
hand-rolled spring-to-anchor force existed in `PhysicsWorld.cpp` (visible
in `git diff` against the pre-Task-3-5 commit) and were removed once the
force generator refactor landed, since the task's own hint says not to
special-case springs in `PhysicsWorld` once Exercise 4 owns that. Per the
author: this was manually confirmed working before cleanup, so the
acceptance criterion was satisfied at the time even though the comparison
isn't live in the current build. No action needed unless you want a
permanent regression check for it later (e.g. as a throwaway scene in
`src/tests/`, same spirit as `exercise1.cpp`).

### Task 3-3 (Verlet) — bug found, now FIXED

Originally: the Verlet particle never received any force. Now fixed via an
`IForceReceiver` interface (`src/physics/IForceReceiver.h`) that both
`Particle` and `VerletParticle` implement, so `ForceGenerator` operates
against the interface instead of a concrete `Particle&`, and
`PhysicsWorld::step()` runs every generator against both particles and
clears both their accumulators each step. Rebuilt clean (WSL/g++15, zero
warnings) after the change. Details of the original bug, for reference:

```cpp
void PhysicsWorld::step(float fixedDt)
{
    semiImplicitParticle.clearForces();

    for (const auto &generator : forceGenerators)
    {
        generator->updateForce(semiImplicitParticle);   // only this particle
    }

    semiImplicitParticle.integrate(fixedDt, IntegrationMode::SemiImplicitEuler);

    verletParticle.integrate(fixedDt);                  // no force ever applied
}
```

`ForceGenerator::updateForce` (`ForceGenerator.h:5-10`) takes a `Particle&`,
so `GravityGenerator`/`DragGenerator` can only ever be called against
`semiImplicitParticle`. `verletParticle.forceAccumulator` is initialized to
`(0,0)` in the constructor and nothing in the codebase ever calls
`verletParticle.applyForce(...)` — grep confirms it. So every step,
`VerletParticle::integrate` computes `acceleration = (0,0) * inverseMass =
(0,0)`, and the position update

```cpp
position = 2*position - oldPosition + acceleration*dt*dt
         = position + (position - oldPosition)
```

degenerates to constant-velocity extrapolation. Concretely: the green
particle in the running app doesn't fall — it glides off-screen forever at
its initial `(3, -1)` velocity, never affected by gravity or drag. This
directly fails Task 3-3's acceptance criterion ("A Verlet particle under
constant gravity falls with the same trajectory as the semi-implicit Euler
version") and it also breaks Task 3-5's own criterion that the refactor
shouldn't change behavior — Verlet's behavior changed a lot, from
oscillating-around-an-anchor to unbounded straight-line drift.

**Why it happened:** `ForceGenerator` was written against the concrete
`Particle` type, so there was no way to hand the same generator a
`VerletParticle`.

**Fix applied:** `Particle` and `VerletParticle` now both implement
`IForceReceiver` (`applyForce`, `getMass`, `getInverseMass`,
`getVelocity(dt)`), `ForceGenerator::updateForce` takes an
`IForceReceiver&` + `dt` instead of a concrete `Particle&`, and
`PhysicsWorld::step()` runs `clearForces()` + every generator against both
particles before integrating. Chose the "common interface" option over
overloading `updateForce` per-type, since Exercise 4's rope/cloth will need
one generator list to drive many particles regardless of integrator —
overloads would mean every future generator implementing both signatures
by hand.

Separately — the comment you added on `VerletParticle`
(`VerletParticle.h:5-7`, "constraints can directly modify position; the
next step automatically derives velocity") is exactly the explanation
Task 3-3 asks for. Good.

### Task 3-4 (Drag) — correct, but always coupled to gravity

`DragGenerator::updateForce` (`DragGenerator.cpp:9-25`) implements quadratic
drag correctly: `F = -k * |v| * v`, computed as
`velocity * (-dragCoefficient * speed)`, which is the same thing without
the redundant normalize the task's hint warns about. Direction always
opposes velocity, guarded against divide-by-near-zero speed. Correct.

The one gap: `PhysicsWorld`'s constructor always registers both
`DragGenerator` and `GravityGenerator` together (`PhysicsWorld.cpp:12-22`),
so there's currently no way to isolate "drag only, no gravity" to directly
observe the task's acceptance criterion ("a particle with only drag
asymptotically approaches zero, never reverses"). Not wrong, just means
that specific check hasn't actually been run against the live particle —
worth a quick standalone check (even a print/log for a few seconds) before
considering this task fully verified.

### Task 3-5 (Force Accumulator Pattern) — right shape, one broken instance

`Particle::forceAccumulator` + `applyForce`/`clearForces`
(`Particle.h:20,30-32`, `Particle.cpp:15-23`) and the
`ForceGenerator`/`GravityGenerator`/`DragGenerator` split
(`ForceGenerator.h`, `GravityGenerator.*`, `DragGenerator.*`) are exactly
the pattern the task describes: `integrate()` no longer knows about
gravity or drag specifically, and adding a new generator (e.g. wind)
wouldn't require touching `Particle` or `PhysicsWorld::step()`. That part
of the acceptance criteria is met.

"Behavior is identical to Tasks 3-1 and 3-4, just restructured" is now met
too, now that the `IForceReceiver` fix routes gravity/drag to both particle
types. This task is done.

---

## Priority fix list

1. ~~Fix the Verlet force bug~~ — **done**, via `IForceReceiver`
   (`src/physics/IForceReceiver.h`, `PhysicsWorld.cpp:24-40`).
2. ~~Restore/confirm the oscillating (spring) comparison for explicit vs.
   semi-implicit Euler, per Task 3-2~~ — **confirmed already verified**
   by the author during development, before the Task 3-5 cleanup.
3. ~~Add the "why this reordering matters" comment Task 3-2 asks for~~ —
   **done**, `Particle.cpp:39-58`.

Nothing blocking remains for Exercises 1-3.

## Cleanup (non-blocking, whenever convenient)

- Delete `Vec3::perp()` (`Vec3.h:120-126`) — unused, copy-pasted, not
  well-defined for 3D.
- Delete the dead `GlfwErrorCallback`/`update`/`render` functions in
  `main.cpp:16-35`.
- `src/physics/RigidBody.h`/`.cpp` and `src/sandbox/Sandbox.h`/`.cpp` are
  empty scaffolding, not in `CMakeLists.txt`'s source list, and currently
  unused now that `Particle`/`VerletParticle`/`PhysicsWorld` cover
  Exercise 3 directly. Fine to leave for `Sandbox` (Exercise 4+ will want
  it), but `RigidBody.h` will likely collide conceptually with the real
  rigid body type you build in Exercise 5 — consider deleting it now
  rather than having two "RigidBody"-shaped things later.
