# Task 4-4: Cloth Grid

**Module:** Exercise 4 — Springs & Particle Constraints
**Goal:** Extend the rope into a 2D grid of constrained particles — cloth.

## Concept
Cloth is a grid of particles with structural constraints (horizontal and
vertical neighbors, like the rope) plus optionally shear constraints
(diagonal neighbors) and bend constraints (particles two apart) for more
realistic stiffness against folding.

## Task
- Build an `M x N` grid of particles, each connected to its horizontal and vertical neighbors via distance constraints
- Anchor the top row (or top two corners) with `inverseMass = 0`
- Run the constraint relaxation loop (from Task 4-3) over all constraints each step
- Render it as a wireframe grid (lines between connected particles) using your debug draw from Exercise 2
- Add a simple interaction: let the user drag a particle with the mouse (optional but recommended — it's the most satisfying way to feel whether your solver is stable)

## Acceptance Criteria
- [ ] The cloth hangs realistically from its anchored row and drapes/sways under gravity
- [ ] It doesn't explode or oscillate wildly at rest (a sign of too few iterations or a bug in the constraint solve)
- [ ] Adding diagonal shear constraints (optional stretch goal) visibly reduces the cloth's tendency to collapse into a degenerate shape

## Hints
- Start small (e.g. 8x8) — grid constraint counts grow fast, and you want to be iterating quickly while you debug stability, not waiting on a 40x40 grid every frame.
