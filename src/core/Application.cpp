#include "core/Application.h"

#include <algorithm>
#include <cstdio>

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <thread>
#include <chrono>

namespace
{
    void glfwErrorCallback(int error, const char *description)
    {
        std::fprintf(
            stderr,
            "GLFW Error %d: %s\n",
            error,
            description);
    }
}

Application::Application()
{
}

Application::~Application()
{
    shutdown();
}

bool Application::initialize()
{
    glfwSetErrorCallback(glfwErrorCallback);

    if (!glfwInit())
    {
        std::fprintf(stderr, "Failed to initialize GLFW\n");
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(
        width,
        height,
        "Ali Physics Engine",
        nullptr,
        nullptr);

    if (!window)
    {
        std::fprintf(stderr, "Failed to create GLFW window\n");
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(window);

    // Enable VSync.
    glfwSwapInterval(1);

    if (!gladLoadGL(glfwGetProcAddress))
    {
        std::fprintf(stderr, "Failed to initialize GLAD\n");
        glfwDestroyWindow(window);
        window = nullptr;
        glfwTerminate();
        return false;
    }

    if (!renderer.initialize(window))
    {
        std::fprintf(stderr, "Failed to initialize renderer\n");
        glfwDestroyWindow(window);
        window = nullptr;
        glfwTerminate();
        return false;
    }

    return true;
}

void Application::shutdown()
{
    if (window)
    {
        renderer.shutdown();

        glfwDestroyWindow(window);
        window = nullptr;

        glfwTerminate();
    }
}

int Application::run()
{
    if (!initialize())
    {
        return -1;
    }

    constexpr double fixedDt = 1.0 / 60.0;
    constexpr double maxFrameDt = 0.25;

    double accumulator = 0.0;
    double previousTime = glfwGetTime();

    while (!glfwWindowShouldClose(window))
    {
        double currentTime = glfwGetTime();

        double frameDt = currentTime - previousTime;
        previousTime = currentTime;

        // Prevent a long pause from causing an excessive
        // number of physics updates.
        frameDt = std::min(frameDt, maxFrameDt);

        accumulator += frameDt;

        glfwPollEvents();

        // Fixed-timestep update.
        while (accumulator >= fixedDt)
        {
            onUpdate(static_cast<float>(fixedDt));
            accumulator -= fixedDt;
        }

        int displayWidth;
        int displayHeight;

        glfwGetFramebufferSize(window, &displayWidth, &displayHeight);

        glViewport(0, 0, displayWidth, displayHeight);

        glClearColor(0.08f, 0.08f, 0.10f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Render once per display frame.
        renderer.beginFrame();

        onRender();

        renderer.endFrame();

        glfwSwapBuffers(window);
    }

    return 0;
}

void Application::onUpdate(float fixedDt)
{
    // Default: nothing to update.
    (void)fixedDt;
}

void Application::onRender()
{
    // Default rendering demonstrates that the application
    // and renderer are working.

    renderer.drawLine(
        Vec2(-5.0f, 0.0f),
        Vec2(5.0f, 0.0f),
        Color(1.0f, 0.0f, 0.0f));

    renderer.drawCircle(
        Vec2(0.0f, 0.0f),
        2.0f,
        Color(0.0f, 1.0f, 0.0f));

    renderer.drawBox(
        Vec2(3.0f, 1.0f),
        Vec2(1.0f, 0.5f),
        0.5f,
        Color(0.0f, 0.5f, 1.0f));
}