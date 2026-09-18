# Task 3-1: Particle Under Gravity (Explicit Euler)

**Module:** Exercise 3 — Particles & Integrators
**Goal:** The simplest possible simulation: one point mass falling under gravity, integrated the naive way.

## Concept
Explicit (forward) Euler: `position += velocity * dt; velocity += acceleration * dt;`
using the velocity/acceleration from the *start* of the step. It's the
simplest integrator and, as you'll see in Task 3-2, not the one you actually
want.

## Task
In `src/physics/RigidBody.h` (or a new minimal `Particle` type if you'd
rather keep it separate for now), add:
- `position`, `velocity` (`Vec2`)
- `mass`, `inverseMass` (store inverse mass — you'll need `0` inverse mass for infinite-mass/static objects later; document that convention now)
- An `integrate(float dt)` method implementing explicit Euler using a constant gravity acceleration (e.g. `(0, -9.81)`)

Add one particle to `PhysicsWorld`, step it each fixed update, and draw it
with `drawCircle` from Exercise 2.

## Acceptance Criteria
- [ ] The particle visibly falls under gravity
- [ ] Position after `t` seconds roughly matches `p0 + v0*t + 0.5*g*t^2` for the first several steps
- [ ] `inverseMass == 0` means the particle never moves regardless of forces (you'll rely on this for static/anchor points in Exercise 4)

## Hints
- Don't add drag, springs, or collision yet — this task is just gravity and one particle, in isolation.
