# Task 8-3: Friction

**Module:** Exercise 8 — Collision Resolution
**Goal:** Add Coulomb friction so bodies slow down sliding against each other instead of sliding forever.

## Concept
After resolving the *normal* impulse (Task 8-1), compute relative velocity
again and find its component *tangent* to the contact normal (perpendicular
to it). Coulomb friction: the friction impulse magnitude is capped at
`μ * normalImpulseMagnitude` (friction can't exceed a coefficient times how
hard the surfaces are pressed together). If the tangential velocity is
small enough to be fully cancelled within that cap, that's **static
friction**; otherwise it's clamped, and that's **kinetic/dynamic friction**.

```
tangent = (relativeVelocity - dot(relativeVelocity, normal) * normal).normalized()
jt = -dot(relativeVelocity, tangent) / (invMassA + invMassB)
jt = clamp(jt, -mu * j, mu * j)   // j = normal impulse magnitude from Task 8-1
frictionImpulse = jt * tangent
```

## Task
- Implement this after the normal impulse step, per manifold/contact point
- Add a `friction` coefficient per body (combine both bodies' values — document your rule, e.g. `sqrt(muA * muB)`, a common choice)
- Test: a box sliding across a static floor should decelerate and stop; a box on a very low-friction floor should slide much further

## Acceptance Criteria
- [ ] A sliding box decelerates smoothly and comes to rest (doesn't stop instantly, doesn't slide forever)
- [ ] `friction = 0` reproduces frictionless sliding (no tangential deceleration)
- [ ] High friction (e.g. `1.0`+) causes a box to stop almost immediately on contact
- [ ] A box resting on a slight incline (if you test one) either stays put (static friction holding) or slides (exceeds static friction), consistent with the coefficient

## Hints
- Compute and apply the friction impulse using the *same* per-contact-point loop as the normal impulse, not as a separate global pass — they need to see each other's updated velocities within the same solver iteration (this becomes more precise once Task 8-4 adds angular effects).
