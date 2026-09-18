# Task 10-4: Spring/Motor Joint

**Module:** Exercise 10 — Joints & Constraints
**Goal:** Give a joint a driven behavior — either soft (spring-like, not a rigid constraint) or actively powered (a motor driving toward a target velocity).

## Concept
- **Soft joint**: instead of fully solving the constraint to zero error every step, blend in stiffness/damping terms (like Exercise 4's spring, but expressed in the impulse solver — this is essentially the same idea Exercise 15's XPBD formalizes with "compliance")
- **Motor**: add a target relative velocity (linear along a prismatic axis, or angular for a revolute joint) and solve an additional impulse (separately clamped by a max motor force/torque) that drives toward it, independent of the main constraint

## Task
- Pick at least one: add spring behavior to your distance joint (Task 10-1) with stiffness/damping parameters, **or** add a motor to your revolute joint (Task 10-2) that drives constant rotation up to a max torque, **or** both
- For a motor: solve a separate 1D velocity constraint targeting the desired relative angular (or linear) velocity, with its impulse clamped to `[-maxForce*dt, maxForce*dt]` so it can't apply unlimited force
- For a spring joint: don't fully correct the constraint error each iteration — scale the correction by a stiffness factor, and add a damping term opposing the rate of change of the constraint error

## Acceptance Criteria
- [ ] A motorized revolute joint spins the attached body at (approximately) the target angular velocity when unloaded, and visibly struggles/stalls if you load it beyond its max torque (e.g. attach a heavy body or add resistance)
- [ ] A spring distance joint oscillates around its target distance rather than snapping to it rigidly, and damping reduces the oscillation over time
- [ ] Both behaviors are tunable at runtime (stiffness/damping, or target velocity/max force) — an ImGui slider from Exercise 2 is a natural fit here

## Hints
- This task is a good checkpoint to build a small demo scene combining several joint types — e.g. a simple ragdoll-like chain of revolute joints, or a motorized wheel/pendulum — to sanity check everything from Exercise 5 through here works together.
