# Task 14-4: 3D Contact Generation & Resolution

**Module:** Exercise 14 — 3D Collision Detection
**Goal:** Wire GJK+EPA into your existing pipeline: broad phase (extend Exercise 6/12 to 3D AABBs) → GJK/EPA narrow phase → your Exercise 8/9 impulse solver, now operating in 3D.

## Task
- Extend your broad phase (grid, SAP, or BVH from Exercise 6/12 — whichever you kept) to 3D AABBs (add a z-axis; the algorithms themselves don't fundamentally change)
- Dispatch narrow phase: sphere-sphere/sphere-AABB use Task 14-1's direct tests (faster, exact); general convex shapes use GJK (Task 14-2) + EPA (Task 14-3) when GJK reports overlap
- Generate at least one contact point per collision (position: for a simple version, the midpoint between the deepest points of each shape along the EPA normal is an acceptable approximation — full multi-point manifold generation in 3D, matching Task 7-4's edge-clipping, is a stretch goal, not required here)
- Feed contacts into your Exercise 8/9 solver, generalized to `Vec3` linear and angular impulses (same structure as Task 8-4's 2D formulas, with `Vec3` cross products instead of the 2D scalar cross)

## Acceptance Criteria
- [ ] A 3D box dropped onto a 3D static floor (box or ground plane) lands and comes to rest, using GJK/EPA-generated contacts through your existing solver
- [ ] A sphere and a box colliding produce plausible bounce/rest behavior
- [ ] Multiple 3D bodies falling into a pile behave reasonably (some jitter is expected without a full multi-point 3D manifold — note it as a known limitation rather than chasing perfect 3D stacking stability here)
- [ ] Your 2D pipeline (Exercises 5-10) is untouched and still works — confirms you extended/generalized rather than broke the existing code

## Hints
- If 3D stacking is unstable, that's an expected, known limitation of single-point-per-contact 3D collision (real engines generate multiple contact points per 3D face-face collision, which is genuinely more work than this task asks for) — don't sink excessive time chasing perfect 3D stacking stability here; it's explicitly out of scope for this task.
