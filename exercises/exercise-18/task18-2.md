# Task 18-2: Interactive Sandbox Tooling

**Module:** Exercise 18 — Capstone: God-Level Demo
**Goal:** Turn your Sandbox into an actual interactive tool — spawn, inspect, drag, and delete bodies live, and tune world parameters without recompiling.

## Task
Using ImGui (already wired up since Exercise 2), build a small in-app
toolkit:
- Spawn bodies by clicking in the world (choose shape/size/mass via an ImGui panel first)
- Click-drag an existing body (apply a spring-like force toward the mouse position while dragging — a simple, satisfying way to "grab" physics objects)
- Delete a body (click to select, then a delete action)
- Live sliders for global world settings: gravity, solver iteration count, default restitution/friction — reflecting immediately in the running simulation
- Save/load buttons wired to Task 18-1's serialization

## Acceptance Criteria
- [ ] You can build a non-trivial scene (several bodies, at least one joint if convenient) entirely through the UI, with no code changes/recompiles
- [ ] Dragging a body feels physically reasonable (doesn't fight the solver or produce explosive velocities)
- [ ] Changing gravity/iteration count/restitution live visibly changes behavior of the running scene immediately
- [ ] A built scene can be saved and reloaded through the UI

## Hints
- This is the tool you'll actually use to build Task 18-3's demo scene, so prioritize whatever makes *that* easier over covering every conceivable feature.
