#include "core/Application.h"

#include <algorithm>
#include <cstdio>

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <imgui.h>

#include "scenes/GravityScene.h"

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

    registerScenes();

    return true;
}

void Application::registerScenes()
{
    scenes.push_back(std::make_unique<GravityScene>());
}

void Application::shutdown()
{
    if (window)
    {
        // Destroy scenes before the renderer and the GL context, in
        // case a scene ever holds a GPU resource.
        scenes.clear();
        activeSceneIndex = -1;

        renderer.shutdown();

        glfwDestroyWindow(window);
        window = nullptr;

        glfwTerminate();
    }
}

bool Application::hasActiveScene() const
{
    return activeSceneIndex >= 0 &&
           activeSceneIndex < static_cast<int>(scenes.size());
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
    if (hasActiveScene())
    {
        scenes[activeSceneIndex]->onUpdate(fixedDt);
    }
}

void Application::onRender()
{
    if (!hasActiveScene())
    {
        renderMenu();
        return;
    }

    Scene &active = *scenes[activeSceneIndex];

    active.onRender(renderer);

    // Small always-on-top overlay so you can get back to the menu.
    ImGui::SetNextWindowPos(ImVec2(10.0f, 10.0f), ImGuiCond_Always);

    ImGui::Begin(
        "##sceneControls",
        nullptr,
        ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_AlwaysAutoResize);

    ImGui::Text("%s", active.getName());

    bool goBack = ImGui::Button("Back to Menu");

    ImGui::End();

    // Checked outside the window so the key works regardless of
    // which ImGui window currently has focus.
    if (ImGui::IsKeyPressed(ImGuiKey_Escape, false))
    {
        goBack = true;
    }

    if (goBack)
    {
        active.onExit();
        activeSceneIndex = -1;
    }
}

void Application::renderMenu()
{
    ImGui::SetNextWindowPos(ImVec2(20.0f, 20.0f), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(360.0f, 0.0f), ImGuiCond_FirstUseEver);

    ImGui::Begin("AliPhysicsEngine - Scenes");

    ImGui::Text("Choose a scene:");
    ImGui::Separator();

    int requested = -1;

    for (int i = 0; i < static_cast<int>(scenes.size()); ++i)
    {
        if (ImGui::Selectable(scenes[i]->getName()))
        {
            requested = i;
        }
    }

    ImGui::End();

    // Deferred so onEnter runs outside the ImGui window scope.
    if (requested >= 0)
    {
        scenes[requested]->onEnter();
        activeSceneIndex = requested;
    }
}