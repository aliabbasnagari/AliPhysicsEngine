// AliPhysicsEngine - baseline entry point.
// Opens a window and clears the screen every frame. Nothing else lives here
// yet on purpose: the game loop, debug draw and engine systems are built up
// step by step in exercises/ (see exercises/README.md).

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <cstdio>
#include <algorithm>

#include "core/Application.h"

#include "tests/exercise1.cpp"

int main()
{
    Application app;
    return app.run();
}
