#include "scenes/EulerComparisonScene.h"

#include <cfloat>
#include <cmath>
#include <memory>

#include <imgui.h>

#include "graphics/Renderer.h"
#include "physics/AnchoredSpringGenerator.h"

void EulerComparisonScene::onEnter()
{
    world.clear();
    explicitParticle = nullptr;
    semiImplicitParticle = nullptr;

    explicitTrail.clear();
    semiImplicitTrail.clear();

    for (int i = 0; i < energyHistoryLength; ++i)
    {
        explicitEnergyHistory[i] = 0.0f;
        semiImplicitEnergyHistory[i] = 0.0f;
    }
    energyHistoryOffset = 0;

    // restLength = 0 makes this a true 2D isotropic harmonic
    // oscillator: F = -k * (position - anchor).
    world.addForceGenerator(
        std::make_unique<AnchoredSpringGenerator>(
            anchor,
            0.0f,
            springConstant));

    explicitParticle = world.createParticle(
        anchor + Vec2(initialDisplacement, 0.0f),
        Vec2(0.0f, 0.0f),
        1.0f,
        IntegrationMode::ExplicitEuler);

    semiImplicitParticle = world.createParticle(
        anchor + Vec2(initialDisplacement, 0.0f),
        Vec2(0.0f, 0.0f),
        1.0f,
        IntegrationMode::SemiImplicitEuler);
}

float EulerComparisonScene::computeEnergy(const Particle &particle) const
{
    Vec2 offset = particle.position - anchor;

    float kineticEnergy =
        0.5f * (particle.velocity.x * particle.velocity.x +
                particle.velocity.y * particle.velocity.y);

    float potentialEnergy =
        0.5f * springConstant *
        (offset.x * offset.x + offset.y * offset.y);

    // mass = 1 for both particles, so KE above omits the mass term.
    return kineticEnergy + potentialEnergy;
}

void EulerComparisonScene::onUpdate(float fixedDt)
{
    world.step(fixedDt);

    explicitTrail.push_back(explicitParticle->position);
    if (explicitTrail.size() > trailLength)
    {
        explicitTrail.pop_front();
    }

    semiImplicitTrail.push_back(semiImplicitParticle->position);
    if (semiImplicitTrail.size() > trailLength)
    {
        semiImplicitTrail.pop_front();
    }

    explicitEnergyHistory[energyHistoryOffset] =
        computeEnergy(*explicitParticle);

    semiImplicitEnergyHistory[energyHistoryOffset] =
        computeEnergy(*semiImplicitParticle);

    energyHistoryOffset = (energyHistoryOffset + 1) % energyHistoryLength;
}

void EulerComparisonScene::drawTrail(
    Renderer &renderer,
    const std::deque<Vec2> &trail,
    const Color &color) const
{
    for (std::size_t i = 1; i < trail.size(); ++i)
    {
        renderer.drawLine(trail[i - 1], trail[i], color);
    }
}

void EulerComparisonScene::onRender(Renderer &renderer)
{
    Color explicitColor(1.0f, 0.35f, 0.3f);     // red — the unstable one
    Color semiImplicitColor(0.3f, 0.85f, 0.4f); // green — the stable one
    Color dimExplicit(0.4f, 0.15f, 0.13f);
    Color dimSemiImplicit(0.13f, 0.35f, 0.17f);

    drawTrail(renderer, explicitTrail, dimExplicit);
    drawTrail(renderer, semiImplicitTrail, dimSemiImplicit);

    renderer.drawCircle(anchor, 0.12f, Color(1.0f, 1.0f, 1.0f));

    renderer.drawLine(anchor, explicitParticle->position, explicitColor);
    renderer.drawLine(anchor, semiImplicitParticle->position, semiImplicitColor);

    renderer.drawCircle(explicitParticle->position, 0.15f, explicitColor);
    renderer.drawCircle(semiImplicitParticle->position, 0.15f, semiImplicitColor);

    // Own ImGui window — Scene::onRender only gets a Renderer&, but
    // ImGui is a global immediate-mode API, so any scene can draw
    // into it directly.
    ImGui::SetNextWindowPos(ImVec2(10.0f, 60.0f), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(340.0f, 0.0f), ImGuiCond_FirstUseEver);

    ImGui::Begin("Energy");

    ImGui::TextColored(
        ImVec4(1.0f, 0.35f, 0.3f, 1.0f),
        "Explicit Euler:      %.2f J",
        computeEnergy(*explicitParticle));

    ImGui::TextColored(
        ImVec4(0.3f, 0.85f, 0.4f, 1.0f),
        "Semi-Implicit Euler: %.2f J",
        computeEnergy(*semiImplicitParticle));

    ImGui::Separator();

    ImGui::PlotLines(
        "Explicit",
        explicitEnergyHistory,
        energyHistoryLength,
        energyHistoryOffset,
        nullptr,
        0.0f, FLT_MAX,
        ImVec2(0.0f, 60.0f));

    ImGui::PlotLines(
        "Semi-Implicit",
        semiImplicitEnergyHistory,
        energyHistoryLength,
        energyHistoryOffset,
        nullptr,
        0.0f, FLT_MAX,
        ImVec2(0.0f, 60.0f));

    if (ImGui::Button("Reset"))
    {
        onEnter();
    }

    ImGui::End();
}