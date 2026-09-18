# Task 2-1: Fixed Timestep Loop

**Module:** Exercise 2 — Application Shell & Render Loop
**Goal:** A game loop where physics always advances in fixed-size steps, independent of rendering frame rate.

## Concept
If you step physics by the raw frame delta time, the simulation becomes
non-deterministic and unstable (a lag spike produces a huge, unstable step).
The standard fix is the **accumulator pattern**: accumulate real elapsed
time, then drain it in fixed-size chunks (e.g. 1/60s), calling your update
function once per chunk. Render once per frame regardless of how many fixed
steps ran.

## Task
In `src/main.cpp` (this will move into `Application` in Task 2-3, so don't
over-invest in the exact location yet):
- Pick a fixed timestep (e.g. `1.0f / 60.0f`)
- Track an accumulator of leftover time
- Each frame: measure real delta time (via `glfwGetTime()`), add to accumulator
- While accumulator >= fixed timestep: call an `update(fixedDt)` stub and subtract the timestep from the accumulator
- Clamp the per-frame delta time you add to the accumulator (e.g. max 0.25s) so a debugger breakpoint or window-drag stall doesn't cause a huge catch-up burst ("spiral of death")
- Render once per frame after the accumulator loop, regardless of how many update() calls happened

## Acceptance Criteria
- [ ] `update()` is called at a consistent rate close to 60Hz regardless of actual frame rate
- [ ] Artificially stalling the loop (e.g. `Sleep`/`sleep_for` for 2 seconds) does not cause a multi-second freeze from catch-up updates afterward
- [ ] Rendering still happens every frame even if zero or multiple `update()` calls happened that frame

## Hints
- Print the number of `update()` calls per frame temporarily to sanity check the behavior, then remove the prints.
