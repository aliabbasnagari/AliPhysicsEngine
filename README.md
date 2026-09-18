
# AliPhysicsEngine
Simple Physics Engine with Dear ImGui + GLFW + OpenGL

## Project layout

```
src/
  core/       Application lifecycle
  graphics/   Renderer
  math/       Vec2, Vec3, Vec4, Mat4, Quaternion
  physics/    PhysicsWorld, RigidBody
  sandbox/    Test scenes
  main.cpp    Entry point / test app
external/     Vendored dependencies (glfw, glad, imgui)
```

The `src/` tree above is currently a skeleton; `src/main.cpp` is the first
working test app, used to verify that GLFW, glad and Dear ImGui build and
link correctly before the engine/physics code is filled in.

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

Launching the app opens a window titled "AliPhysicsEngine - Test App" with a
Dear ImGui overlay showing frame timing and the ImGui demo window, confirming
the GLFW/glad/ImGui pipeline is working end to end.
