# Task 16-4: Custom Allocator / Object Pool

**Module:** Exercise 16 — Data-Oriented Performance
**Goal:** Eliminate per-frame heap allocation churn for short-lived physics data (contacts, manifolds, broad-phase pair lists) using a pool/arena allocator.

## Concept
If your contact/manifold/pair-list code calls `new`/`malloc` (directly or
via a growing `std::vector`/`std::map` that reallocates) every frame, you're
paying allocator overhead and fragmenting memory for data that's entirely
recreated and discarded every single step. A **pool allocator** (fixed-size
slots, reused every frame) or an **arena/bump allocator** (a big
pre-allocated block, "freed" by just resetting a cursor to zero each frame)
eliminates that churn entirely.

## Task
- Using Task 16-1's profiler (extend it if needed, e.g. counting allocations per frame, or just reasoning about your own code's `new`/`vector::push_back` patterns), identify at least one genuinely per-frame-recreated allocation source (contact manifolds, broad-phase candidate pairs, and joint solver scratch data are typical candidates)
- Implement a pool allocator (fixed pool of reusable slots, an active/free list) or an arena allocator (bump-pointer, reset each frame) for that data
- Replace the corresponding `new`/growing-container usage with allocations from your pool/arena
- Verify no leaks and no stale-data bugs (a classic pool bug: reusing a slot before all references to the old occupant are done with it, especially with your Task 9-1 persistent manifolds needing to survive *across* frames while other per-frame data doesn't — make sure your pool's lifetime assumptions actually match each piece of data's real lifetime)

## Acceptance Criteria
- [ ] The targeted allocation source no longer calls `new`/reallocates per frame in the steady state (verify via a debugger, an allocation counter, or a tool like Valgrind/Tracy/similar if available)
- [ ] Physics behavior is unchanged — this is a pure performance refactor, not a behavior change
- [ ] You have before/after numbers (even just "allocations per frame: before N, after ~0" is a meaningful, valid result — you don't need microsecond-precision timing to make this task's point)

## Hints
- Be honest about which data is genuinely per-frame-scratch (safe for a reset-every-frame arena) vs. persistent across frames (Task 9-1's manifolds, joints) — mixing these up is the most common bug in this task, not the allocator logic itself.
