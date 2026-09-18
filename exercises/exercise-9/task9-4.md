# Task 9-4: Sleeping Bodies

**Module:** Exercise 9 — Stable Contacts & Solver Quality
**Goal:** Stop simulating bodies that have settled and aren't moving — critical for both performance (Exercise 16 cares a lot about this) and eliminating residual micro-jitter in "resting" stacks.

## Concept
Track how long a body's linear and angular velocity have stayed below a
small threshold. Once that's true for long enough, mark it **asleep**: skip
integration, force application, and solver work for it (though it should
still be included as an immovable obstacle for *other* bodies' collision
checks, similar to a static body). Any contact or force from an awake body
should **wake** a sleeping one back up.

## Task
- Add a sleep timer and `isAwake` flag to `RigidBody`
- Each step: if a body's linear speed and angular speed are both below thresholds, increment its sleep timer; otherwise reset it to zero
- Once the timer exceeds a duration (e.g. 0.5s), set `isAwake = false` and zero its velocities
- Skip force application/integration for sleeping bodies; skip running solver iterations for contacts where *both* bodies are asleep
- Wake a sleeping body immediately if a moving/awake body's contact manifold touches it, or if an external force/impulse is applied to it directly

## Acceptance Criteria
- [ ] A settled stack of boxes goes to sleep after the configured duration (verify by e.g. color-tinting sleeping bodies in your debug draw)
- [ ] Dropping a new body onto a sleeping stack wakes the bodies it touches (and only those, not unrelated far-away sleeping bodies)
- [ ] A sleeping body remains a valid obstacle — an awake body colliding with it still resolves the contact correctly, it just doesn't move on its own
- [ ] CPU-side, iterating a sleeping body should be measurably cheaper than an awake one (skip the real work, don't just no-op deep inside it)

## Hints
- This is also where "islands" become a natural next optimization (grouping connected bodies so sleep/wake and later parallel solving, Exercise 17, operate per-island) — you don't need full island tracking yet, a simple per-body/per-contact wake propagation is enough here.
