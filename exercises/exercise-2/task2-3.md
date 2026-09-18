# Task 2-3: Application Class

**Module:** Exercise 2 — Application Shell & Render Loop
**Goal:** Move the window/loop/render plumbing out of `main.cpp` and into a reusable `Application` class, so `main.cpp` becomes a two-line entry point.

## Task
In `src/core/Application.h`/`.cpp`, create an `Application` class that:
- Owns the `GLFWwindow*`, handles init/shutdown of GLFW, glad, and the debug renderer from Task 2-2
- Exposes `run()`, which contains the fixed-timestep loop from Task 2-1
- Exposes virtual (or callback-based — your choice) `onUpdate(float fixedDt)` and `onRender()` hooks that a subclass (or the Sandbox, later) can override
- Handles clean shutdown on window close

Reduce `src/main.cpp` to creating an `Application` (or subclass) and calling
`run()`.

## Acceptance Criteria
- [ ] `main.cpp` no longer contains GLFW/glad/ImGui setup code directly
- [ ] The app still opens a window, runs the fixed loop, and renders, with identical behavior to before this task
- [ ] `Application` can be instantiated and run with no code outside `core/` and `main.cpp` needing to know about GLFW directly

## Hints
- Don't make this more generic than you need (no plugin systems, no config files) — you have exactly one consumer (this engine) for a long while.
