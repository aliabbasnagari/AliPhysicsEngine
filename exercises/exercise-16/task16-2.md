# Task 16-2: Structure-of-Arrays Refactor

**Module:** Exercise 16 — Data-Oriented Performance
**Goal:** Restructure body storage for cache-friendly iteration — a classic data-oriented design (DOD) technique, and a good chance to feel its actual performance impact on your own code rather than take it on faith.

## Concept
**Array-of-Structures (AoS)** (what you likely have: `std::vector<RigidBody>`,
each body a big struct with every field) means iterating "just positions"
for e.g. broad-phase AABB updates still pulls the *entire* struct (velocity,
inertia, shape data, everything) into cache line by line, wasting bandwidth
on fields you're not touching. **Structure-of-Arrays (SoA)** stores each
field in its own contiguous array (`positions[]`, `velocities[]`, etc.),
so a pass that only touches positions only streams position data.

## Task
- Using Task 16-1's profiler, identify at least one hot per-body loop (e.g. AABB computation, integration) as a candidate
- Refactor `RigidBody` storage in `PhysicsWorld` from AoS to SoA for at least the fields touched by that hot loop (you do not need to convert *everything* — a partial, targeted conversion is fine and more realistic than an all-or-nothing rewrite)
- Keep a stable way to reference "a body" (an index or handle) so the rest of the engine (collision, joints, solver) isn't forced to change wholesale in this one task
- Re-measure the specific loop you converted with Task 16-1's profiler, before and after, on the same stress-test scene

## Acceptance Criteria
- [ ] You have before/after timing numbers for the specific converted loop, from your own profiler, on your own hardware
- [ ] The rest of the engine (collision, solving, joints) still works correctly after the refactor — SoA changes memory layout, not physics behavior
- [ ] You can explain, with your own numbers, whether the conversion helped, and if it didn't help as much as expected, have a hypothesis why (e.g. the loop wasn't actually memory-bandwidth-bound to begin with)

## Hints
- It's a completely legitimate, useful outcome if this refactor turns out *not* to matter much for your specific engine's current bottleneck — that's real information Task 16-1 should have hinted at. Don't force a SoA conversion everywhere just because it's the "advanced" technique; apply it where your own data says it matters.
