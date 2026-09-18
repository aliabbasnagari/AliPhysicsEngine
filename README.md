
# AliPhysicsEngine
A physics engine built from scratch, in C++, as a learning project (GLFW +
glad + Dear ImGui for the window/rendering/debug-UI shell).

This repo is deliberately a **blank baseline**. The engine itself does not
exist yet - it's built up incrementally by working through
[exercises/](exercises/README.md), which take you from vector math up to a
full 2D/3D rigid body engine with joints, GJK/EPA, soft bodies and more.

## Project layout

```
src/
  core/       Application lifecycle
  graphics/   Renderer
  math/       Vec2, Vec3, Vec4, Mat4, Quaternion
  physics/    PhysicsWorld, RigidBody
  sandbox/    Test scenes
  main.cpp    Entry point (currently just opens a window and clears the screen)
external/     Vendored dependencies (glfw, glad, imgui)
exercises/    The learning curriculum - start at exercises/README.md
```

Every file under `src/` besides `main.cpp` is an empty stub. `main.cpp` is
the minimal runnable baseline: it opens a GLFW window with a glad-loaded GL
context and clears it every frame. That's it - no game loop abstraction, no
ImGui, no physics. You build all of that yourself in the exercises.

## Dependencies

- [GLFW](https://www.glfw.org/) (`external/glfw`) - windowing, input, OpenGL context
- [glad](https://glad.dav1d.de/) (`external/glad`) - OpenGL function loader
- [Dear ImGui](https://github.com/ocornut/imgui) (`external/imgui`) - debug UI

These are vendored under `external/` and built as part of this project; no
separate installation is required. CMake fetches nothing from the network.

## Building

Requires CMake 3.16+ and a C++17 compiler (MSVC, MinGW/GCC, or Clang).

### WSL / Linux

GLFW needs the X11 development headers. On Ubuntu/Debian (including WSL):

```sh
sudo apt update
sudo apt install -y build-essential cmake libx11-dev libxrandr-dev \
    libxinerama-dev libxcursor-dev libxi-dev libxext-dev libgl-dev
```

`CMakeLists.txt` disables GLFW's Wayland backend (`GLFW_BUILD_WAYLAND OFF`)
since `wayland-scanner`/`wayland-protocols` aren't installed by default on
WSL; GLFW falls back to X11, which WSLg (Windows 11) provides out of the
box, so GUI windows show up without extra setup.

### Using the Makefile

```sh
make        # configure + build (Debug by default)
make run    # build and run the test app
make clean  # remove the build directory
```

Build type can be overridden, e.g. `make BUILD_TYPE=Release`.

### Using CMake directly

```sh
cmake -S . -B build
cmake --build build
```

The executable is written to `build/bin/` (e.g. `build/bin/AliPhysicsEngine.exe`
on Windows).

## Running

Launching the app opens a blank dark-grey window - confirming the
GLFW/glad pipeline works end to end. Nothing else happens until you start
building on it.

## Learning path

Start at [exercises/README.md](exercises/README.md) for the full curriculum
and suggested order. As you add source files while working through it,
remember to add them to `CMakeLists.txt`'s `add_executable(AliPhysicsEngine ...)`
call so they get built.
