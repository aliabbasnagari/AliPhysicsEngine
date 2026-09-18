# Task 8-2: Positional Correction

**Module:** Exercise 8 — Collision Resolution
**Goal:** Stop bodies from sinking into each other over time by directly correcting position, on top of the velocity impulse from Task 8-1.

## Concept
Impulse resolution fixes *velocity*, but a resting body under continuous
gravity re-penetrates slightly every step before the next collision
response — without correction, it slowly sinks. The standard fix: after
velocity resolution, push the two bodies apart along the normal,
proportional to inverse mass, by some fraction of the penetration depth
(not all of it at once — that causes jitter). This is the same
inverse-mass-weighted split you used for Task 4-2's distance constraint.

## Task
- Implement **linear projection**: `correction = max(penetration - slop, 0) / (invMassA + invMassB) * percent * normal`, then move A by `-correction * invMassA` and B by `+correction * invMassB`
- `slop` is a small allowed overlap (e.g. `0.01` units) to prevent correcting jitter-inducing tiny penetrations
- `percent` is a fraction (e.g. `0.2-0.8`) — correct only part of the penetration per step, not all at once, to avoid overshoot/jitter
- Apply this after the velocity impulse from Task 8-1, for every manifold, every step

## Acceptance Criteria
- [ ] A body resting on a static floor no longer visibly sinks over time (stays at a stable resting height, within `slop`)
- [ ] Stacked bodies (try 2-3 boxes stacked) don't visibly jitter or vibrate at rest
- [ ] Increasing `percent` toward 1.0 makes correction faster but introduces more visible jitter/popping — confirm you can feel this tradeoff by tuning it

## Hints
- This is a **position** hack, not a physically "real" force — it's how virtually every real-time engine handles this (full physical correctness here is what Exercise 9's more advanced techniques and Exercise 15's XPBD move toward).
