# Task 2-2: Debug Draw

**Module:** Exercise 2 — Application Shell & Render Loop
**Goal:** A simple way to draw shapes (lines, circles, boxes) so every future physics exercise is visually inspectable.

## Concept
You don't need a "real" renderer (meshes, shaders, materials) to see physics
work — you need lines and circles drawn every frame. Dear ImGui's
`ImDrawList` (via `ImGui::GetBackgroundDrawList()`) is a fast way to get 2D
immediate-mode drawing without writing GL shader code yet.

## Task
In `src/graphics/Renderer.h`/`.cpp`, wire up ImGui (init/shutdown/frame
begin-end, mirroring what the original test app in `main.cpp` did) and add
a small debug draw API, e.g.:
- `drawLine(Vec2 a, Vec2 b, Color color)`
- `drawCircle(Vec2 center, float radius, Color color, int segments = 24)`
- `drawBox(Vec2 center, Vec2 halfExtents, float angleRadians, Color color)`
- Decide on and document your world-to-screen mapping (e.g. 1 world unit = N pixels, Y up vs Y down) — you'll use this constantly

## Acceptance Criteria
- [ ] Calling `drawCircle` and `drawLine` from `main.cpp` inside the render step produces visible shapes on screen
- [ ] A box drawn with a non-zero angle visibly rotates
- [ ] World-to-screen convention is written down somewhere (a comment is fine) so future tasks don't have to reverse-engineer it

## Hints
- `ImGui::GetBackgroundDrawList()->AddCircle(...)` / `AddLine(...)` are the ImGui calls you want; no ImGui window is required for background draw lists.
