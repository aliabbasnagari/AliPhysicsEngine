# Task 5-2: Moment of Inertia

**Module:** Exercise 5 — Rigid Body Fundamentals
**Goal:** Compute correct rotational inertia for circles and boxes so torque produces physically plausible spin.

## Concept
Moment of inertia `I` is rotational mass's analogue — it determines how
much torque is needed for a given angular acceleration (`τ = I·α`, the
rotational version of `F = m·a`). It depends on shape and mass
*distribution*, not just total mass. Standard formulas (about the center of
mass, 2D, solid uniform density):
- Solid disc/circle of radius `r`: `I = 0.5 * m * r^2`
- Solid rectangle of width `w`, height `h`: `I = (1/12) * m * (w^2 + h^2)`

## Task
- Add functions/factories that compute `inertia` (and its inverse) from mass + shape dimensions using the formulas above, for both circle and box shapes
- Wire these into however you construct `RigidBody`s (a factory function `RigidBody::createCircle(mass, radius, ...)` / `createBox(mass, w, h, ...)` is a clean way to do this)
- Verify: a body with `inverseMass == 0` should also get `inverseInertia == 0` (an immovable body shouldn't spin either) — decide whether this is automatic or something the caller sets explicitly, and be consistent

## Acceptance Criteria
- [ ] A circle and a box of equal mass have different computed inertia (unless dimensions coincidentally match)
- [ ] Doubling the radius of a circle (same mass) noticeably increases its inertia — apply equal torque to two such bodies and confirm the larger one spins up more slowly
- [ ] Static bodies never rotate under torque, confirming `inverseInertia == 0` is respected

## Hints
- These formulas assume uniform density and rotation about the center of mass — don't generalize to arbitrary pivot points yet, that's not needed until joints (Exercise 10).
