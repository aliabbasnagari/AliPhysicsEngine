# Task 15-3: Cloth via PBD

**Module:** Exercise 15 — Position-Based & Extended Position-Based Dynamics
**Goal:** Rebuild Exercise 4's cloth grid through the generalized XPBD solver, and add bending constraints for more realistic behavior.

## Concept
Structural constraints (grid neighbors, as in Exercise 4) alone let cloth
fold completely flat/collapse unrealistically, because nothing resists
*bending*. A **bending constraint** connects particles two apart (skipping
one), with a target distance/angle that resists folding — this is what
gives cloth a believable "stiffness against creasing" beyond just not
stretching.

## Task
- Rebuild the Exercise 4/Task 4-4 cloth grid as a set of `Constraint` objects (Task 15-1) run through your XPBD solver (Task 15-2)
- Add structural constraints (direct neighbors) and bending constraints (two-apart neighbors), each with independently tunable compliance
- Tune structural compliance low (stiff, resists stretching) and bending compliance higher (allows folding/draping) — observe how independently tunable compliance per constraint *type* gives you material control that classic PBD's "iteration count" lever didn't
- Add the mouse-drag interaction from Task 4-4 if you didn't already, or bring it forward now

## Acceptance Criteria
- [ ] Cloth drapes and folds more realistically than the Exercise 4 version, specifically showing resistance to sharp creasing due to bending constraints
- [ ] Structural and bending compliance can be tuned independently, with visibly different effects (stiffen structural only vs. stiffen bending only)
- [ ] The cloth remains stable (no explosion/NaN positions) under reasonable interaction (dragging, dropping onto a body) across a range of compliance values

## Hints
- If cloth explodes when you drag it hard, check your position correction isn't unbounded per substep — clamping the maximum position change per constraint per substep is a common, pragmatic stability safeguard worth adding.
