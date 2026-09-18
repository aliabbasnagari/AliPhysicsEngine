# Task 11-3: Conservative Advancement CCD

**Module:** Exercise 11 — Continuous Collision Detection & Raycasting
**Goal:** Fix Task 11-2's tunneling case using swept-shape continuous collision detection.

## Concept
Instead of only checking shape overlap at the end of a step, treat a
fast-moving body's motion as a **swept shape** across the step (for a
circle, this is a ray from its start to end position, expanded by its
radius — reuse Task 11-1's raycasting directly). If the raycast against
other (typically static/slow) bodies finds a hit *before* the body's full
step distance, clamp the body's movement to just before that hit point
this step (or resolve the collision at that sub-step time), instead of
letting it move the full distance.

## Task
- Identify which bodies need CCD each step: e.g. bodies whose movement distance this step exceeds some fraction of their own size ("fast movers") — don't run CCD on every body every step, that's wasteful and Exercise 16 will care about this cost
- For each fast mover, raycast (Task 11-1) from its start to intended end position against the rest of the world (skip other fast-moving dynamic bodies for now — dynamic-vs-dynamic CCD is a harder, optional extension)
- If a hit is found before the full movement distance, stop the body at (just before) the hit point for this step, and let the normal discrete collision response (Exercise 8) take over from there next step
- Re-run Task 11-2's exact tunneling scene and confirm it no longer tunnels

## Acceptance Criteria
- [ ] Task 11-2's reproducible tunneling scene no longer tunnels once CCD is enabled for the fast body
- [ ] Slow-moving bodies are unaffected (still use pure discrete detection — verify CCD isn't accidentally running, and costing time, on everything)
- [ ] A CCD-stopped body doesn't visibly "stick" awkwardly at the wall — normal collision resolution should take over smoothly next step
- [ ] You understand and can state the limitation: this is a simplified approach (stop-and-resolve-next-step), not a full time-of-impact solver — good enough for most gameplay, but document that as a known limitation rather than presenting it as complete

## Hints
- A visual debug toggle showing which bodies are being treated as "fast" this frame (e.g. a color tint) makes it easy to confirm your fast-mover detection threshold is triggering exactly when you expect.
