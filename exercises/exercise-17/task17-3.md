# Task 17-3: Deterministic Simulation

**Module:** Exercise 17 — Multithreading & Determinism
**Goal:** Make repeated runs of the same scene from the same initial state produce bit-identical results — a prerequisite for Task 17-4's lockstep replay, and generally for reproducible debugging.

## Concept
Common determinism breakers in a physics engine: variable timestep (already
solved by Exercise 2's fixed timestep — confirm it's actually being used
everywhere, not just approximated); iteration order that depends on
non-deterministic container ordering (e.g. iterating a `std::unordered_map`
keyed by pointer/address); floating-point operations whose result can
differ by compiler/platform/optimization flags (e.g. FMA fusion, `-ffast-math`
style reassociation); and — the one Exercise 17-1/17-2 just introduced —
**thread scheduling**, where the *order* floating-point additions happen in
across threads can change the result of summed/accumulated values (floating
point addition isn't associative).

## Task
- Audit your engine for determinism breakers: any container iteration order that depends on memory addresses/insertion timing rather than a stable key (e.g. body ID, not pointer); confirm compiler flags don't allow reordering that changes results run to run
- For the parallel solver from Task 17-2: ensure island processing order (or however you merge/apply results across threads) is deterministic — e.g. sort islands by a stable key before dispatching, don't rely on "whichever thread finishes first"
- Build a test: run the exact same scene twice (same seed/initial conditions), N steps each, and compare final body positions/velocities bit-for-bit (or hash the full state each run and compare hashes)
- Repeat with your parallel solver from Task 17-2 enabled and confirm it's *still* bit-identical across runs — this is the harder bar parallelism raises

## Acceptance Criteria
- [ ] Two runs of the identical scene (single-threaded) produce bit-identical final state
- [ ] Two runs of the identical scene with the parallel solver (Task 17-2) enabled also produce bit-identical final state, run to run
- [ ] You've identified and documented (as comments, or a short note) at least one determinism hazard you found and fixed, specific to your own codebase
- [ ] You understand this guarantees determinism only on the *same* machine/build/compiler — cross-platform bit-determinism (matching a different CPU architecture or compiler) is a much harder, mostly out-of-scope bar; don't chase it unless you specifically want to

## Hints
- A cheap, effective bit-identical-state check: serialize all body positions/velocities to a byte buffer each run and compare with `memcmp`, or hash the buffer — don't rely on eyeballing printed floats, which hides small but real bit-level differences.
