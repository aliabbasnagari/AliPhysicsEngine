# Evaluation — Exercise 5

**Verdict: PASS.** All acceptance criteria for Exercise 5 are met. This was a code and scene review. I did not build or run the app.

## Task 5-1 — RigidBody Data
- `RigidBody` holds position, rotation, linear and angular velocity, mass and inertia with their inverses, a `ShapeType` tag, and `radius` or `halfExtents`. It is a flat class, as the hint asks.
- The sign and static conventions are documented at the top of [RigidBody.h](../../src/physics/RigidBody.h). Rotation is radians, counter-clockwise positive, +Y up. Static means `inverseMass == 0 && inverseInertia == 0`, and an `isStatic()` helper means call sites don't re-check the inverses.
- `Transform` with `localToWorld` and `worldToLocal` is a clean answer to "world-space transform usable by debug draw". Both rotations are correct, and `worldToLocal` is the inverse rotation.
- `drawBody` in [BodyDrawing.h](../../src/scenes/BodyDrawing.h) draws local X/Y axes and a spoke on circles. This makes rotation visible, including for circles, which are otherwise rotationally symmetric.
- The constructor is private and everything goes through the factories, so you can't construct a body with an arbitrary explicit inertia. Task 5-1 asked for that "for now", but Task 5-2 supersedes it by computing inertia in the factories. This is a reasonable call. Add a public overload only if a test needs a non-standard inertia.

## Task 5-2 — Moment of Inertia
- The circle uses `0.5·m·r²`. The box uses `(1/12)·m·(w²+h²)`, and it correctly converts half-extents to full width and height first. That conversion is the usual mistake here, and it is handled.
- `mass <= 0` gives zero inertia, so zero inverse inertia automatically. This resolves the task's "automatic or explicit" question and applies it consistently to both shapes.
- `RigidBodyScene` demonstrates the criteria. Two circles of equal mass and equal torque differ only in radius, so the bigger one spins up more slowly. A static box under force and torque stays put.

## Task 5-3 — Force, Torque & Angular Integration
- `applyForceAtPoint` accumulates the linear force and a torque of `r.cross(force)`. The existing `Vec2::cross` is already the scalar 2D form, so no extra helper was needed. The task's hint about a 3D cross doesn't apply here.
- `integrate` is semi-implicit Euler for both linear and angular motion, then clears both accumulators. Static bodies exit early and clear their accumulators, so a force applied to a static body can't pile up and be released later.
- `RigidBodyScene` covers all four criteria side by side:
  - a force through the centre of mass,
  - the same force at the right edge,
  - the mirrored point, which reverses the spin,
  - torque only.
- The scene applies its loads through a `ForceGenerator`. This puts them after the accumulators are cleared and before integration, so it follows the same pass as gravity and doesn't bypass the world step. Force arrows are drawn from the application points, which makes the demo easy to read.

## Task 5-4 — PhysicsWorld Step
- Ownership is `std::vector<std::unique_ptr<RigidBody>>`, with non-owning pointers handed out. Pointers stay stable across `push_back`, which is why `unique_ptr` is a better choice than a plain `vector<RigidBody>` here. The pointer-removal contract is documented: the pointer is invalidated, unknown pointers are ignored, and don't call it during `step()`.
- `step()` clears all accumulators, runs the force generators over particles, Verlet particles and rigid bodies, then integrates. Rigid bodies reuse the Exercise 3 `GravityGenerator` through `IForceReceiver`, so no gravity code was duplicated.
- `FallingBodiesScene` covers the criteria:
  - circles and boxes with different masses fall at the same rate,
  - one body is already spinning, so its spin should stay constant under gravity,
  - one has sideways velocity,
  - one is static, so gravity must not move it,
  - one body is removed mid-run, to show the others are unaffected.
- The scene clears and restarts itself when bodies leave the screen. This is expected with no collisions yet.
- The task mentions `Sandbox`, but this project uses a `Scene` interface registered in `Application::registerScenes()`. The scenes only see `Renderer` and `fixedDt`, with no GLFW or ImGui, so the intent of the criterion is met.

## Minor notes
- None blocking.
- `rotation` is never wrapped, so it grows without bound on long-running spinners. That is harmless for now. Wrap or normalize it if it ever feeds a trig-heavy or precision-sensitive path in the collision exercises.
- Rigid bodies take no part in the distance-constraint solver. That is fine until joints in Exercise 10.
- `integrate` is not tied to a `dt` check, and `getVelocity(float)` and `getPosition(float)` ignore their argument. This is acceptable for the `IForceReceiver` interface.

Ready to move on to Exercise 6.
