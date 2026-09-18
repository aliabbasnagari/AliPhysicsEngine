# Task 15-4: Soft-Body Blob

**Module:** Exercise 15 — Position-Based & Extended Position-Based Dynamics
**Goal:** A squishy, volume-preserving soft body (think: a jelly cube or blob) using distance constraints plus a volume constraint.

## Concept
A ring/mesh of particles connected only by distance constraints can be
squashed flat with no resistance (distance constraints don't prevent
collapsing the *interior* area/volume). A **volume constraint** (2D: area;
3D: volume) adds a target area/volume for the shape and corrects all
participating particles' positions to push back toward it when compressed
or stretched — this is what makes a soft body feel like it has "stuff
inside it" pushing back, not just an outline.

## Task
- Build a simple soft body shape: a ring of particles (2D, e.g. an octagon) or a small tetrahedron-mesh blob (3D, harder — 2D is a perfectly good scope for this task) connected by distance constraints around the perimeter (and a few internal cross-bracing distance constraints for shape stability)
- Add a volume/area constraint (as another `Constraint` implementation, per Task 15-1's interface) using the shoelace formula for 2D polygon area (or a tetrahedron volume sum for 3D), correcting all involved particles' positions to restore target area/volume when it deviates
- Drop the blob onto a static floor and let it land, squash on impact, and settle back toward its rest shape

## Acceptance Criteria
- [ ] The blob visibly squashes on impact and recovers back toward (not necessarily exactly, some residual deformation is fine) its original shape/volume
- [ ] Removing the volume constraint (leave only the perimeter distance constraints) visibly changes behavior — the shape can be squashed much flatter/collapse more, confirming the volume constraint is doing real work
- [ ] The blob doesn't lose significant volume/area permanently after repeated impacts (some drift is expected/acceptable; total collapse or unbounded growth is not)

## Hints
- This task is a satisfying capstone for the particle/constraint side of the curriculum (Exercises 3, 4, 15) — a good one to spend extra polish time on if you want a genuinely fun demo to show off before moving into performance/architecture territory (Exercise 16+).
