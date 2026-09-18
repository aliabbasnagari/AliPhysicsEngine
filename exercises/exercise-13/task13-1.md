# Task 13-1: 3D Inertia Tensors

**Module:** Exercise 13 — 3D Math & Rigid Bodies
**Goal:** Compute the inertia tensor for boxes, spheres, and cylinders about their center of mass — the 3D generalization of Task 5-2.

## Concept
In 3D, resistance to rotation differs by axis, so inertia is a 3x3 symmetric
matrix. For a body's own local axes aligned with its principal axes of
inertia (true for these standard shapes at their center), the tensor is
**diagonal** — just three numbers, `Ixx, Iyy, Izz`. Standard formulas (solid,
uniform density, about center of mass):
- Solid sphere, radius `r`: `Ixx = Iyy = Izz = (2/5) * m * r^2`
- Solid box, dimensions `w, h, d`: `Ixx = (1/12)*m*(h^2+d^2)`, `Iyy = (1/12)*m*(w^2+d^2)`, `Izz = (1/12)*m*(w^2+h^2)`
- Solid cylinder, radius `r`, height `h`, axis along y: `Iyy = 0.5*m*r^2`, `Ixx = Izz = (1/12)*m*(3*r^2 + h^2)`

The inverse you actually need each step is the **inverse inertia tensor in
world space**, which changes as the body rotates: `I_world^-1 = R * I_local^-1 * R^T`, where `R` is the body's current rotation matrix (from Task 1-4's `toMat4`, or the 3x3 part of it).

## Task
- Add factory functions computing the local (body-space) diagonal inertia tensor for sphere, box, and cylinder shapes, plus its inverse (trivial for a diagonal matrix — just invert each diagonal entry, using `0` for infinite-mass axes)
- Add a function to compute the current world-space inverse inertia tensor from the local inverse tensor and the body's current orientation (quaternion → rotation matrix, per the formula above)
- Verify: a box with very different width/height/depth has visibly different `Ixx/Iyy/Izz` — confirm resistance to rotation differs by axis (e.g. it's easier to spin a long thin rod end-over-end around its short axis than to spin it like a propeller around its long axis)

## Acceptance Criteria
- [ ] Diagonal tensor values match hand-computed values for at least one test case per shape
- [ ] The world-space inverse inertia tensor changes as the body rotates (verify by printing it before/after rotating a test body) and matches the local tensor when orientation is identity
- [ ] A long thin box given equal torque about its long axis vs a short axis visibly spins up at different rates, confirming the tensor is actually being used per-axis, not just as an averaged scalar

## Hints
- You don't need a full general (non-diagonal, arbitrary-shape) inertia tensor calculator — these three shapes cover everything you need for the rest of the curriculum. Don't build more generality than the tasks ahead actually require.
