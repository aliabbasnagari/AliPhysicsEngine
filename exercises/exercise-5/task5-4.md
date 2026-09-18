# Task 5-4: PhysicsWorld Step

**Module:** Exercise 5 — Rigid Body Fundamentals
**Goal:** A proper `PhysicsWorld` that owns a collection of rigid bodies and steps them all — the container the rest of the engine (collision, joints) plugs into.

## Task
In `src/physics/PhysicsWorld.h`/`.cpp`:
- Store a collection of `RigidBody` (or pointers/handles to them — pick an ownership model and stick with it; `std::vector<RigidBody>` is fine to start)
- `addBody(...)` / a way to spawn bodies into the world
- `step(float fixedDt)`: clear all accumulators → apply gravity (as a force, using the Task 3-5 pattern generalized to rigid bodies) → integrate all bodies
- Wire this into your `Application`'s fixed update from Exercise 2
- Rebuild your Exercise 4 rope/cloth demo (or a simple new scene) using `PhysicsWorld` + `RigidBody` end to end, in `Sandbox`, to confirm the whole pipeline (loop → world step → debug draw) works together

## Acceptance Criteria
- [ ] Spawning several circle and box bodies with different masses, all falling under gravity, behave correctly and independently
- [ ] Removing a body from the world (if you support it) doesn't crash or affect other bodies
- [ ] `Sandbox` can set up a scene (spawn some bodies) and the `Application` loop drives it without `Sandbox` needing to know about GLFW/ImGui directly

## Hints
- No collision yet — bodies will fall through each other and off-screen. That's expected; Exercise 6 starts detecting overlaps.
