# Task 8-1: Normal Impulse & Restitution

**Module:** Exercise 8 — Collision Resolution
**Goal:** Make two colliding bodies bounce off each other along the contact normal, with a tunable "bounciness."

## Concept
An impulse `J` is an instantaneous change in momentum: `Δv = J * inverseMass`.
For a contact, solve for the normal impulse magnitude that produces the
desired **relative velocity** along the normal after the collision, using
the **coefficient of restitution** `e` (0 = perfectly inelastic/no bounce,
1 = perfectly elastic/full bounce):

```
relativeVelocity = velocityB - velocityA  (linear only, for now — no rotation yet, that's Task 8-4)
velocityAlongNormal = dot(relativeVelocity, normal)
j = -(1 + e) * velocityAlongNormal / (invMassA + invMassB)
impulse = j * normal
velocityA -= impulse * invMassA
velocityB += impulse * invMassB
```
Only apply the impulse if bodies are approaching (`velocityAlongNormal < 0`) — don't pull already-separating bodies together.

## Task
- Using the `Manifold` from Task 7-4, implement this resolution for the linear-velocity-only case
- Add a `restitution` property per body (or per manifold, combined from both bodies — document your combination rule, e.g. `min` or average)
- Test: drop a circle onto a static floor (a large static box) and watch it bounce

## Acceptance Criteria
- [ ] `restitution = 1` produces a bounce that returns to (approximately) its original height
- [ ] `restitution = 0` produces no bounce — the body stops dead at the contact
- [ ] Two dynamic bodies colliding head-on exchange velocity correctly (verify against the textbook 1D elastic collision formula for equal masses: velocities simply swap when `e = 1`)
- [ ] Already-separating bodies are left untouched (no impulse applied)

## Hints
- Don't add positional correction yet (Task 8-2) — with this task alone, bodies will slowly sink into each other due to accumulated floating-point/gravity drift before the next collision is detected. That's expected and exactly what the next task fixes.
