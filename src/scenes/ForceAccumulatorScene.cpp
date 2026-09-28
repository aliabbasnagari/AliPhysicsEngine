#include "scenes/ForceAccumulatorScene.h"

#include <memory>

#include <imgui.h>

void ForceAccumulatorScene::onEnter()
{
    relaunch();
}

void ForceAccumulatorScene::relaunch()
{
    // clear() wipes both particles and generators, so both rebuild
    // together from current UI state.
    world.clear();

    particle = world.createParticle(launchPosition, launchVelocity, 1.0f);
    particle->setIntegrationMode(IntegrationMode::SemiImplicitEuler);

    rebuildGenerators();

    trail.clear();
    trail.push_back(particle->position);
}

void ForceAccumulatorScene::rebuildGenerators()
{
    // The entire demonstration lives in this function. It only calls
    // clearForceGenerators() / addForceGenerator(). It never touches
    // Particle::integrate() or PhysicsWorld::step() — those don't
    // know or care which forces exist.
    world.clearForceGenerators();

    gravityGenerator = nullptr;
    dragGenerator = nullptr;
    windGenerator = nullptr;

    if (gravityEnabled)
    {
        gravityGenerator = static_cast<GravityGenerator *>(
            world.addForceGenerator(
                std::make_unique<GravityGenerator>(gravity)));
    }

    if (dragEnabled)
    {
        dragGenerator = static_cast<DragGenerator *>(
            world.addForceGenerator(
                std::make_unique<DragGenerator>(
                    dragCoefficient, DragMode::Linear)));
    }

    if (windEnabled)
    {
        // WindGenerator did not exist when Particle or PhysicsWorld
        // were written. Registering it costs one call, right here.
        windGenerator = static_cast<WindGenerator *>(
            world.addForceGenerator(
                std::make_unique<WindGenerator>(windForce)));
    }
}

void ForceAccumulatorScene::onUpdate(float fixedDt)
{
    if (!particle)
        return;

    world.step(fixedDt);

    trail.push_back(particle->position);
    if (trail.size() > trailLength)
        trail.pop_front();

    if (particle->position.y < -10.0f ||
        particle->position.x > 15.0f ||
        particle->position.x < -15.0f)
    {
        relaunch();
    }
}

void ForceAccumulatorScene::drawTrail(
    Renderer &renderer,
    const std::deque<Vec2> &trailPoints,
    const Color &color) const
{
    for (std::size_t i = 1; i < trailPoints.size(); ++i)
    {
        renderer.drawLine(trailPoints[i - 1], trailPoints[i], color);
    }
}

void ForceAccumulatorScene::onRender(Renderer &renderer)
{
    if (!particle)
        return;

    drawTrail(renderer, trail, Color(0.35f, 0.35f, 0.4f));
    renderer.drawCircle(particle->position, 0.15f, Color(0.4f, 0.8f, 1.0f));

    ImGui::SetNextWindowPos(ImVec2(10.0f, 60.0f), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(380.0f, 0.0f), ImGuiCond_FirstUseEver);

    ImGui::Begin("Force Accumulator Pattern");

    ImGui::TextWrapped(
        "Each checkbox only calls clearForceGenerators()/"
        "addForceGenerator() on the world. Particle::integrate() and "
        "PhysicsWorld::step() are never touched.");

    ImGui::Separator();

    bool changed = false;

    changed |= ImGui::Checkbox("Gravity", &gravityEnabled);
    if (gravityEnabled)
    {
        ImGui::SameLine();
        ImGui::SetNextItemWidth(120.0f);
        changed |= ImGui::SliderFloat("g", &gravity.y, -20.0f, -1.0f);
    }

    changed |= ImGui::Checkbox("Drag (linear)", &dragEnabled);
    if (dragEnabled)
    {
        ImGui::SameLine();
        ImGui::SetNextItemWidth(120.0f);
        changed |= ImGui::SliderFloat("k##drag", &dragCoefficient, 0.0f, 5.0f);
    }

    changed |= ImGui::Checkbox("Wind (new!)", &windEnabled);
    if (windEnabled)
    {
        ImGui::SameLine();
        ImGui::SetNextItemWidth(120.0f);
        changed |= ImGui::SliderFloat("wind x", &windForce.x, -10.0f, 10.0f);
    }

    if (changed)
    {
        // Re-registers with current slider values. Note: the particle
        // itself is untouched — only the world's generator list
        // changes, mid-flight, without a reset.
        rebuildGenerators();
    }

    ImGui::Separator();

    if (ImGui::Button("Preset: Task 3-1 (gravity only)"))
    {
        gravityEnabled = true;
        dragEnabled = false;
        windEnabled = false;
        relaunch();
    }

    ImGui::SameLine();

    if (ImGui::Button("Preset: Task 3-4 (drag only)"))
    {
        gravityEnabled = false;
        dragEnabled = true;
        windEnabled = false;
        launchVelocity = Vec2(3.0f, 0.0f);
        relaunch();
    }

    if (ImGui::Button("Relaunch"))
    {
        relaunch();
    }

    ImGui::Text(
        "Registered generators: %d",
        static_cast<int>(gravityEnabled) +
            static_cast<int>(dragEnabled) +
            static_cast<int>(windEnabled));

    ImGui::End();
}