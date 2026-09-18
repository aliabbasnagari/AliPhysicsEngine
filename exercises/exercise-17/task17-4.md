# Task 17-4: Lockstep Replay Harness

**Module:** Exercise 17 — Multithreading & Determinism
**Goal:** Prove your determinism work (Task 17-3) with something concrete and useful: record a session's inputs, replay them, and confirm you get the same simulation back — the same core mechanism deterministic-lockstep multiplayer networking relies on.

## Concept
If your simulation is truly deterministic given the same initial state and
the same sequence of inputs (forces applied, bodies spawned, mouse
drags/joints changed, etc. — not raw physics state, which is derived), you
can reconstruct an entire session from just: the initial state, and a
timestamped log of inputs. This is "lockstep": every client simulates
locally from the same inputs and reaches the same state, rather than
constantly syncing full state over the network.

## Task
- Define an `Input` type covering everything external that can affect your simulation deterministically (spawn body, apply force/impulse, drag a joint anchor, etc.) — this should already roughly match whatever interactions your Sandbox scenes support
- Record: log every input with the exact fixed-step frame number it occurred on, alongside the initial scene setup
- Replay: reset to the initial scene setup, then step the simulation, feeding back each recorded input at its exact recorded frame number instead of live user input
- Verify: replaying a recorded session produces bit-identical final state (Task 17-3's check) to the original live run

## Acceptance Criteria
- [ ] A recorded session, when replayed, visually reproduces the same simulation (bodies move identically)
- [ ] Final state after replay is bit-identical to the original run's final state (reuse Task 17-3's comparison method)
- [ ] Replay works correctly with the parallel solver (Task 17-2) enabled, confirming Task 17-3's parallel-determinism guarantee holds under a real recorded scenario, not just a synthetic test
- [ ] You can articulate the gap between this and real networked lockstep (this task assumes both "sides" already agree on inputs at the right frame — actual networking has to additionally handle input delay, packet loss and desync recovery, which are out of scope here but worth naming as the next layer up)

## Hints
- This is a genuinely satisfying capstone for the "god tier" performance/architecture tier — a working record/replay is tangible proof that Tasks 16 and 17's harder, less visually flashy work (profiling, SoA, SIMD, islands, determinism) actually holds together end to end.
