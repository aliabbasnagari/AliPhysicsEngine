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

static void GlfwErrorCallback(int error, const char *description)
{
    std::fprintf(stderr, "GLFW error %d: %s\n", error, description);
}

static void update(float dt)
{
    // Physics update will go here later.
}

static void render(GLFWwindow *window)
{
    int displayWidth, displayHeight;
    glfwGetFramebufferSize(window, &displayWidth, &displayHeight);

    glViewport(0, 0, displayWidth, displayHeight);

    glClearColor(0.08f, 0.08f, 0.10f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

int main()
{

    // Test cases
    runTest11();
    runTest12();
    runTest13();
    runTest14();

    Application app;
    return app.run();
}
