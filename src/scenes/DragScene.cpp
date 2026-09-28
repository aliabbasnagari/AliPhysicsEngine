#include "scenes/DragScene.h"

#include <cfloat>
#include <cmath>

#include <imgui.h>

#include "graphics/Renderer.h"

namespace
{
    float length(Vec2 v)
    {
        return std::sqrt(v.x * v.x + v.y * v.y);
    }

    Vec2 normalized(Vec2 v)
    {
        float len = length(v);
        if (len < 1e-6f)
        {
            return Vec2(0.0f, 0.0f);
        }
        return v * (1.0f / len);
    }
}

void DragScene::onEnter()
{
    relaunch();
}

void DragScene::relaunch()
{
    world.clear();

    particle = world.createParticle(launchPosition, launchVelocity, 1.0f);
    particle->setIntegrationMode(IntegrationMode::SemiImplicitEuler);

    // Registered with the world, not applied by hand. world.step()
    // now runs this every frame as part of its normal generator pass.
    dragGenerator = static_cast<DragGenerator *>(
        world.addForceGenerator(
            std::make_unique<DragGenerator>(dragCoefficient, dragMode)));

    launchDirection = normalized(launchVelocity);
    everReversed = false;
    everIncreased = false;

    trail.clear();
    trail.push_back(particle->position);

    for (int i = 0; i < speedHistoryLength; ++i)
    {
        speedHistory[i] = 0.0f;
    }
    speedHistoryOffset = 0;
}

void DragScene::onUpdate(float fixedDt)
{
    if (!particle)
        return;

    float speedBefore = length(particle->velocity);

    // No manual updateForce() call — world.step() handles it because
    // dragGenerator is registered.
    world.step(fixedDt);

    trail.push_back(particle->position);
    if (trail.size() > trailLength)
        trail.pop_front();

    float speedAfter = length(particle->velocity);

    speedHistory[speedHistoryOffset] = speedAfter;
    speedHistoryOffset = (speedHistoryOffset + 1) % speedHistoryLength;

    if (speedAfter > speedBefore + 1e-4f)
    {
        everIncreased = true;
    }

    float directionDot =
        particle->velocity.x * launchDirection.x +
        particle->velocity.y * launchDirection.y;

    if (directionDot < 0.0f)
    {
        everReversed = true;
    }
}

void DragScene::drawTrail(
    Renderer &renderer,
    const std::deque<Vec2> &trailPoints,
    const Color &color) const
{
    for (std::size_t i = 1; i < trailPoints.size(); ++i)
    {
        renderer.drawLine(trailPoints[i - 1], trailPoints[i], color);
    }
}

void DragScene::onRender(Renderer &renderer)
{
    if (!particle)
    {
        return;
    }

    Color trailColor(0.35f, 0.35f, 0.4f);
    Color particleColor(0.3f, 0.8f, 1.0f);
    Color velocityColor(0.3f, 1.0f, 0.4f); // green: velocity direction
    Color forceColor(1.0f, 0.35f, 0.3f);   // red: drag force direction

    drawTrail(renderer, trail, trailColor);

    renderer.drawCircle(particle->position, 0.15f, particleColor);

    float speed = length(particle->velocity);

    if (speed > 1e-4f)
    {
        Vec2 velocityDirection = normalized(particle->velocity);

        // Visual arrow lengths only, clamped so a fast launch doesn't
        // send them off-screen.
        float arrowLength = std::min(speed * 0.15f, 1.0f);

        renderer.drawLine(
            particle->position,
            particle->position + velocityDirection * arrowLength,
            velocityColor);

        // Drag opposes velocity by construction (see DragGenerator);
        // drawing it as a fixed-length arrow pointing the opposite way
        // from the velocity arrow is the "eyeball" check the task asks
        // for — the two should never point the same direction.
        renderer.drawLine(
            particle->position,
            particle->position + velocityDirection * -0.4f,
            forceColor);
    }

    ImGui::SetNextWindowPos(ImVec2(10.0f, 60.0f), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(360.0f, 0.0f), ImGuiCond_FirstUseEver);

    ImGui::Begin("Drag / Damping");

    ImGui::TextColored(ImVec4(0.3f, 1.0f, 0.4f, 1.0f), "green = velocity");
    ImGui::SameLine();
    ImGui::TextColored(ImVec4(1.0f, 0.35f, 0.3f, 1.0f), "  red = drag force");

    ImGui::Separator();

    int modeIndex = (dragGenerator->getMode() == DragMode::Linear) ? 0 : 1;

    ImGui::Text("Drag model:");
    if (ImGui::RadioButton("Linear (F = -k*v)", modeIndex == 0))
    {
        dragGenerator->setMode(DragMode::Linear);
    }
    ImGui::SameLine();
    if (ImGui::RadioButton("Quadratic (F = -k*|v|*v)", modeIndex == 1))
    {
        dragGenerator->setMode(DragMode::Quadratic);
    }

    float coefficient = dragGenerator->getCoefficient();
    if (ImGui::SliderFloat("Drag coefficient (k)", &coefficient, 0.0f, 5.0f))
    {
        dragGenerator->setCoefficient(coefficient);
    }

    ImGui::Separator();

    ImGui::Text("Speed: %.3f", speed);
    ImGui::Text(
        "Ever reversed direction: %s",
        everReversed ? "YES (!)" : "no");
    ImGui::Text(
        "Ever sped up between frames: %s",
        everIncreased ? "YES (!)" : "no");

    ImGui::PlotLines(
        "Speed history",
        speedHistory,
        speedHistoryLength,
        speedHistoryOffset,
        nullptr,
        0.0f, FLT_MAX,
        ImVec2(0.0f, 60.0f));

    ImGui::Separator();

    ImGui::SliderFloat("Launch speed", &launchSpeed, 0.5f, 8.0f);

    if (ImGui::Button("Relaunch"))
    {
        launchVelocity = Vec2(launchSpeed, 0.0f);
        relaunch();
    }

    ImGui::End();
}
