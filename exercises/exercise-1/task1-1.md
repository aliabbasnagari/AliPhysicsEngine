# Task 1-1: Vec2

**Module:** Exercise 1 — Math Foundations
**Goal:** A working 2D vector type you'll use for every particle and 2D rigid body from Exercise 3 onward.

## Concept
A 2D vector is just `(x, y)`. Dot product `a·b = ax*bx + ay*by` gives you
projection/angle information; the "perpendicular" (rotate 90°) is used
constantly in 2D physics for normals and tangents.

## Task
In `src/math/Vec2.h`, implement a `Vec2` struct with:
- Fields `x`, `y` (float)
- Constructors: default (zero), `Vec2(x, y)`
- Operators: `+`, `-` (binary and unary negate), `*` (scalar), `/` (scalar), `+=`, `-=`, `*=`
- `dot(const Vec2&) const`
- `length() const`, `lengthSquared() const`
- `normalized() const` (returns a unit vector; decide and document what happens for a zero-length vector)
- `perp() const` — returns the vector rotated 90° (i.e. `(-y, x)`)
- `operator==` (exact or epsilon-based, your choice)

## Acceptance Criteria
- [ ] `Vec2(3,4).length() == 5`
- [ ] `Vec2(1,0).dot(Vec2(0,1)) == 0`
- [ ] `Vec2(1,0).perp() == Vec2(0,1)`
- [ ] `a.normalized().length()` is `~1` for any non-zero `a`
- [ ] Compiles as a header-only include with no `.cpp`

## Hints
- Keep it a POD-like struct (no virtuals) — it needs to be cheap to copy and pass by value.
