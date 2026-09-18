# Task 3-5: Force Accumulator Pattern

**Module:** Exercise 3 — Particles & Integrators
**Goal:** Replace ad-hoc "gravity + drag hardcoded in integrate()" with a generic, extensible force system — the pattern every later force/spring/constraint will plug into.

## Concept
Instead of hardcoding force logic inside the integrator, particles
accumulate a `forceAccumulator` each step (`clearForces()` → each active
force generator calls `applyForce()` → `integrate()` consumes the
accumulator and resets it to zero for next step). This decouples "what
forces exist" from "how integration works."

## Task
- Add `Vec2 forceAccumulator` to your particle, plus `applyForce(Vec2 force)` and `clearForces()`
- Change `integrate()` to compute acceleration from `forceAccumulator * inverseMass` instead of a hardcoded gravity constant
- Define a small `ForceGenerator` interface (or simplest thing that works — even a `std::function<void(Particle&)>`) with at least two implementations: `GravityGenerator`, `DragGenerator` (reusing Task 3-1/3-4 logic, now expressed as generators)
- `PhysicsWorld::step()` should: clear forces on all particles → run all registered generators → integrate all particles

## Acceptance Criteria
- [ ] Removing/adding a force generator from the world changes particle behavior without touching `integrate()`
- [ ] Behavior is identical to Tasks 3-1 and 3-4 (same trajectory), just restructured
- [ ] Adding a *new* force (e.g. a constant wind force) requires only a new generator, zero changes to `Particle` or `PhysicsWorld::step()`

## Hints
- This is the seam Exercise 4's springs will plug into, so resist the urge to special-case anything about gravity or drag in `PhysicsWorld` itself.
