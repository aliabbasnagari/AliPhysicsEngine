#include "graphics/Renderer.h"

#include <cmath>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <GLFW/glfw3.h>

namespace
{
    ImU32 toImGuiColor(Color color)
    {
        return ImGui::ColorConvertFloat4ToU32(
            ImVec4(
                color.r,
                color.g,
                color.b,
                color.a));
    }
}

bool Renderer::initialize(GLFWwindow *window)
{
    IMGUI_CHECKVERSION();

    ImGui::CreateContext();

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    return true;
}

void Renderer::shutdown()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();

    ImGui::DestroyContext();
}

void Renderer::beginFrame()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void Renderer::endFrame()
{
    ImGui::Render();

    ImGui_ImplOpenGL3_RenderDrawData(
        ImGui::GetDrawData());
}

void Renderer::drawLine(
    Vec2 a,
    Vec2 b,
    Color color)
{
    ImDrawList *drawList =
        ImGui::GetBackgroundDrawList();

    ImVec2 screenA(
        ImGui::GetIO().DisplaySize.x * 0.5f +
            a.x * worldToPixels,

        ImGui::GetIO().DisplaySize.y * 0.5f -
            a.y * worldToPixels);

    ImVec2 screenB(
        ImGui::GetIO().DisplaySize.x * 0.5f +
            b.x * worldToPixels,

        ImGui::GetIO().DisplaySize.y * 0.5f -
            b.y * worldToPixels);

    drawList->AddLine(
        screenA,
        screenB,
        toImGuiColor(color),
        2.0f);
}

void Renderer::drawCircle(
    Vec2 center,
    float radius,
    Color color,
    int segments)
{
    ImDrawList *drawList =
        ImGui::GetBackgroundDrawList();

    ImVec2 screenCenter(
        ImGui::GetIO().DisplaySize.x * 0.5f +
            center.x * worldToPixels,

        ImGui::GetIO().DisplaySize.y * 0.5f -
            center.y * worldToPixels);

    drawList->AddCircle(
        screenCenter,
        radius * worldToPixels,
        toImGuiColor(color),
        segments,
        2.0f);
}

void Renderer::drawBox(
    Vec2 center,
    Vec2 halfExtents,
    float angleRadians,
    Color color)
{
    float c = std::cos(angleRadians);
    float s = std::sin(angleRadians);

    // Local box corners.
    Vec2 corners[4] =
        {
            Vec2(-halfExtents.x, -halfExtents.y),
            Vec2(halfExtents.x, -halfExtents.y),
            Vec2(halfExtents.x, halfExtents.y),
            Vec2(-halfExtents.x, halfExtents.y)};

    // Rotate each corner and translate by center.
    for (int i = 0; i < 4; ++i)
    {
        float x = corners[i].x;
        float y = corners[i].y;

        corners[i] = Vec2(
            center.x + x * c - y * s,
            center.y + x * s + y * c);
    }

    // Draw four edges.
    drawLine(corners[0], corners[1], color);
    drawLine(corners[1], corners[2], color);
    drawLine(corners[2], corners[3], color);
    drawLine(corners[3], corners[0], color);
}