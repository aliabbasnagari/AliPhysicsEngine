# Exercise 2 — Application Shell & Render Loop

**Tier:** 0 — Foundations
**Prerequisites:** Exercise 1

Right now `src/main.cpp` is a bare GLFW+glad window. Before writing any
physics, you need a proper loop (fixed timestep, so simulation is stable
regardless of frame rate), a way to draw debug shapes (circles, lines,
boxes), and somewhere for the engine to actually live (`Application`).

## What you'll build

A fixed-timestep game loop, an immediate-mode debug draw API, and an
`Application` class that owns the window/loop and replaces the raw loop
currently in `main.cpp`.

## Files you'll touch

- `src/core/Application.h` / `.cpp`
- `src/graphics/Renderer.h` / `.cpp`
- `src/main.cpp`

## Tasks

1. [Task 2-1: Fixed Timestep Loop](task2-1.md)
2. [Task 2-2: Debug Draw](task2-2.md)
3. [Task 2-3: Application Class](task2-3.md)
