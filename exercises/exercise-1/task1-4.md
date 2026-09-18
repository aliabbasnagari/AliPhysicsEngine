# Task 1-4: Quaternion

**Module:** Exercise 1 — Math Foundations
**Goal:** A quaternion type for representing 3D orientation without gimbal lock — required for Exercise 13 (3D rigid bodies) onward.

## Concept
A unit quaternion `(x, y, z, w)` represents a rotation. Unlike Euler angles,
composing and integrating rotations with quaternions doesn't suffer gimbal
lock, which is why every serious 3D physics/animation engine uses them
internally.

## Task
In `src/math/Quaternion.h`, implement a `Quaternion` struct with:
- Fields `x, y, z, w`
- `identity()` static factory (`0,0,0,1`)
- `fromAxisAngle(Vec3 axis, float radians)` static factory
- `operator*` for quaternion-quaternion multiplication (composition — check your convention for which side applies first and document it)
- `normalized() const`
- `rotate(const Vec3& v) const` — rotates a vector by this quaternion
- `toMat4() const` — converts to an equivalent rotation `Mat4`

## Acceptance Criteria
- [ ] `Quaternion::identity().rotate(v) == v`
- [ ] `Quaternion::fromAxisAngle({0,0,1}, pi/2).rotate({1,0,0})` is approximately `(0,1,0)`
- [ ] Multiplying two quaternions and rotating a vector matches applying the two rotations sequentially
- [ ] `q.toMat4()` transforming a point gives the same result as `q.rotate()` on that point

## Hints
- Normalize after every multiplication in practice (you'll rely on this once you integrate angular velocity into orientation in Exercise 13) — but don't force-normalize inside every operator; that's a later task's concern.
