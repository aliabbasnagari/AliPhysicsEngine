# Task 18-1: Scene Serialization

**Module:** Exercise 18 — Capstone: God-Level Demo
**Goal:** Save and load a scene (bodies, joints, and enough world config to reproduce it) to/from a file.

## Task
- Pick a format: a simple custom text format, or JSON (a small header-only JSON library is a reasonable dependency to add here if you want one — vendor it under `external/` like your existing dependencies) — either is fine, don't over-engineer the format itself
- Serialize: every body's shape, mass/inertia (or the params to recompute them), initial position/orientation/velocity; every joint's type, connected bodies, and parameters; relevant world settings (gravity, solver iteration count)
- Deserialize: reconstruct an equivalent `PhysicsWorld` from a saved file
- Round-trip test: save a scene, load it back, and confirm it behaves identically (not necessarily bit-identical unless you've done Exercise 17's determinism work — "visually and structurally identical" is the bar here)

## Acceptance Criteria
- [ ] A hand-built Sandbox scene can be saved to a file and reloaded, reproducing the same initial setup
- [ ] Loading a malformed/missing file fails gracefully (a clear error, not a crash)
- [ ] At least one joint type and multiple body shape types round-trip correctly

## Hints
- You don't need this to be a general-purpose engine asset pipeline — it needs to work for your own Sandbox scenes. Scope it to what you'll actually use in Tasks 18-2/18-3.
