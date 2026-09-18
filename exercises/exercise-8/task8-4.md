# Task 8-4: Angular Impulse from Contacts

**Module:** Exercise 8 — Collision Resolution
**Goal:** Make collisions correctly impart *spin*, not just linear velocity change — required for anything that isn't a circle hitting dead-center, which is most collisions.

## Concept
The relative velocity at a contact point isn't just the bodies' center
velocities — it includes each body's rotation: `velocityAtPoint = linearVelocity + angularVelocity × r` (2D cross of scalar ω and vector r gives `(-ω*r.y, ω*r.x)`), where `r` is the vector from center of mass to the contact point. The impulse formula from Task 8-1 generalizes to include each body's `inverseInertia` and the torque the impulse would produce:

```
raCrossN = cross(rA, normal)   // scalar
rbCrossN = cross(rB, normal)
denominator = invMassA + invMassB + invInertiaA * raCrossN^2 + invInertiaB * rbCrossN^2
j = -(1 + e) * velocityAlongNormal / denominator
// apply linear impulse as before, PLUS:
angularVelocityA -= invInertiaA * cross(rA, impulse)
angularVelocityB += invInertiaB * cross(rB, impulse)
```
Friction (Task 8-3) needs the same generalization (its own `raCrossT`/`rbCrossT` terms in the denominator).

## Task
- Update relative velocity computation in both the normal impulse (Task 8-1) and friction (Task 8-3) code to include each body's angular velocity contribution at the contact point
- Update the impulse denominator and the post-impulse update to include the angular terms above, for both normal and friction impulses
- Apply this per contact point when a manifold has two (from Task 7-4) — each point contributes its own impulse

## Acceptance Criteria
- [ ] A ball hitting a box off-center makes the box spin, not just slide
- [ ] A box dropped flat onto a floor (2 contact points) settles flat without rotating, using both points together — using only 1 point would incorrectly tip it
- [ ] A spinning body that hits a wall has its spin visibly affected by the collision (not left untouched)
- [ ] Energy behavior is still sane — bodies don't gain rotational energy from a collision (a common bug sign: a stack of boxes it starts spontaneously vibrating/spinning at rest)

## Hints
- This is the task where stacking either starts looking convincing or starts visibly misbehaving (jitter, sinking, explosive corrections) — if it's unstable, Exercise 9 (persistent manifolds, warm starting, more solver iterations) is specifically about fixing that, so don't over-tune this task chasing perfect stacking yet.
