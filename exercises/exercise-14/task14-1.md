# Task 14-1: Sphere vs Sphere, Sphere vs AABB

**Module:** Exercise 14 — 3D Collision Detection
**Goal:** Warm up with the 3D versions of Exercise 7's easiest cases before tackling GJK.

## Task
- Sphere vs sphere: identical logic to Task 7-1, just with `Vec3` — distance between centers vs sum of radii
- Sphere vs AABB: identical logic to Task 7-2's circle-vs-AABB closest-point approach, extended to 3 axes
- Return the same contact info shape you'll want for the rest of the module: normal, penetration depth, contact point

## Acceptance Criteria
- [ ] Both tests correctly detect overlap/no-overlap for known cases
- [ ] Sphere vs AABB correctly handles the sphere-center-inside-box degenerate case (same as Task 7-2)
- [ ] These integrate into your 3D `PhysicsWorld` step from Task 13-3 and produce visible collision response using your existing Exercise 8 impulse solver (extended to `Vec3`/3D torque, which you already built in Task 13-3)

## Hints
- This task is deliberately easy — it exists so you have a working, visible 3D collision response demo *before* GJK/EPA, so you can isolate GJK/EPA bugs later without also wondering if your 3D solver integration is broken.
