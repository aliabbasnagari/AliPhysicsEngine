// AliPhysicsEngine - baseline entry point.
// Opens a window and clears the screen every frame. Nothing else lives here
// yet on purpose: the game loop, debug draw and engine systems are built up
// step by step in exercises/ (see exercises/README.md).

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <cstdio>

#include "tests/exercise1.cpp"

static void GlfwErrorCallback(int error, const char *description)
{
    std::fprintf(stderr, "GLFW error %d: %s\n", error, description);
}

int main()
{

    // Test cases
    runTest11();
    runTest12();

    glfwSetErrorCallback(GlfwErrorCallback);
    if (!glfwInit())
    {
        std::fprintf(stderr, "Failed to initialize GLFW\n");
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(1280, 720, "AliPhysicsEngine", nullptr, nullptr);
    if (!window)
    {
        std::fprintf(stderr, "Failed to create GLFW window\n");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress))
    {
        std::fprintf(stderr, "Failed to initialize glad\n");
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        int displayWidth, displayHeight;
        glfwGetFramebufferSize(window, &displayWidth, &displayHeight);
        glViewport(0, 0, displayWidth, displayHeight);
        glClearColor(0.08f, 0.08f, 0.10f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
