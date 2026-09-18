# Task 15-2: XPBD

**Module:** Exercise 15 — Position-Based & Extended Position-Based Dynamics
**Goal:** Fix classic PBD's core weakness — its effective stiffness depends on iteration count and timestep, which makes "how stiff is this material" impossible to reason about independent of solver settings — using the compliance-based XPBD formulation.

## Concept
Classic PBD constraints have no real physical stiffness parameter — more
iterations or a smaller timestep silently makes everything *stiffer*.
**XPBD** (Müller et al., 2016) introduces **compliance** `α` (inverse
stiffness, `α = 1/k`; `α = 0` means fully rigid) and an accumulated
Lagrange multiplier `λ` per constraint per step, with a corrected
projection formula:

```
Δλ = (-C - α̃ * λ) / (w1 + w2 + α̃)      where α̃ = α / dt²,  C = constraint error, w = inverse mass
apply position correction using Δλ (same direction/weighting as before)
λ += Δλ    (accumulated across sub-steps within this frame, reset to 0 each new frame)
```
Critically, XPBD is typically run with several **sub-steps** per frame
(each sub-step re-predicting positions and running a small number of
constraint iterations) rather than many iterations on one big step — this
is a different (and more correct) way of spending your iteration budget.

## Task
- Extend your Task 15-1 `Constraint` interface to carry a compliance value and a per-step accumulated `λ`
- Implement the XPBD projection formula for your distance constraint
- Restructure the solver's outer loop to sub-step (e.g. 4-8 sub-steps per frame, each doing predict → a few constraint iterations → velocity update) instead of one big predict + many iterations
- Compare the *same* rope/cloth scene at a few different compliance values and confirm behavior changes predictably (low compliance = stiff/rigid-like, high compliance = soft/stretchy) **independent of** iteration/substep count — vary substep count at fixed compliance and confirm the resting behavior stays consistent, unlike classic PBD

## Acceptance Criteria
- [ ] Compliance `0` (or very close to it) produces near-rigid rope/cloth behavior, closely matching Exercise 4's hard distance constraint
- [ ] Higher compliance values produce visibly softer, stretchier behavior, tunable independent of substep count
- [ ] Doubling the substep count at a fixed compliance value does **not** visibly change the resting stiffness (this is the specific bug XPBD fixes vs. classic PBD — verify it explicitly, don't just assume)
- [ ] `λ` is correctly reset to zero at the start of each new frame's substep sequence (a very common bug: forgetting to reset it causes constraints to drift into being effectively infinitely stiff or completely broken after a few frames)

## Hints
- Implement this as genuinely new code, not a copy-paste of Task 15-1 with a formula swapped in — read the XPBD paper's short "Algorithm 1" pseudocode section if the projection formula's derivation isn't clicking from the summary above; it's a short, readable paper by simulation standards.
