#include "scenes/HookeSpringScene.h"

#include <cfloat>
#include <cmath>
#include <memory>

#include <imgui.h>

namespace
{
    float length(Vec2 v)
    {
        return std::sqrt(v.x * v.x + v.y * v.y);
    }
}

void HookeSpringScene::onEnter()
{
    reset();
}

void HookeSpringScene::reset()
{
    world.clear();

    // mass = 0 -> inverseMass = 0 -> "infinite mass", per Particle's
    // own convention. Forces applied to it do nothing; integrate()
    // skips it entirely.
    anchorParticle =
        world.createParticle(anchorPosition, Vec2(0.0f, 0.0f), 0.0f);

    freeParticle = world.createParticle(
        freeStartPosition, Vec2(0.0f, 0.0f), freeMass);

    freeParticle->setIntegrationMode(
        integrationModeIndex == 0
            ? IntegrationMode::ExplicitEuler
            : IntegrationMode::SemiImplicitEuler);

    gravityGenerator = static_cast<GravityGenerator *>(
        world.addForceGenerator(std::make_unique<GravityGenerator>(gravity)));

    spring = world.addSpring(
        std::make_unique<SpringForceGenerator>(
            *anchorParticle, *freeParticle,
            stiffness, restLength, damping));

    trail.clear();
    trail.push_back(freeParticle->position);

    for (int i = 0; i < energyHistoryLength; ++i)
    {
        energyHistory[i] = 0.0f;
    }
    energyHistoryOffset = 0;
}

float HookeSpringScene::computeTotalEnergy() const
{
    float speed = length(freeParticle->velocity);
    float kinetic = 0.5f * freeParticle->mass * speed * speed;

    // Gravitational PE, using raw (signed) gravity.y so this is a
    // standard m*g*h with height increasing upward. Only the trend
    // over time matters here, not the absolute reference point.
    float gravitationalPotential =
        -gravity.y * freeParticle->mass * freeParticle->position.y;

    Vec2 delta = freeParticle->position - anchorParticle->position;
    float currentLength = length(delta);
    float extension = currentLength - restLength;

    float springPotential = 0.5f * stiffness * extension * extension;

    return kinetic + gravitationalPotential + springPotential;
}

void HookeSpringScene::onUpdate(float fixedDt)
{
    if (!freeParticle)
        return;

    lastFixedDt = fixedDt;

    world.step(fixedDt);

    trail.push_back(freeParticle->position);
    if (trail.size() > trailLength)
    {
        trail.pop_front();
    }

    energyHistory[energyHistoryOffset] = computeTotalEnergy();
    energyHistoryOffset = (energyHistoryOffset + 1) % energyHistoryLength;
}

void HookeSpringScene::drawTrail(
    Renderer &renderer,
    const std::deque<Vec2> &trailPoints,
    const Color &color) const
{
    for (std::size_t i = 1; i < trailPoints.size(); ++i)
    {
        renderer.drawLine(trailPoints[i - 1], trailPoints[i], color);
    }
}

void HookeSpringScene::onRender(Renderer &renderer)
{
    if (!anchorParticle || !freeParticle)
        return;

    drawTrail(renderer, trail, Color(0.35f, 0.35f, 0.4f));

    renderer.drawLine(
        anchorParticle->position,
        freeParticle->position,
        Color(0.6f, 0.6f, 0.65f));

    renderer.drawCircle(anchorParticle->position, 0.12f, Color(1.0f, 1.0f, 1.0f));
    renderer.drawCircle(freeParticle->position, 0.18f, Color(0.3f, 0.8f, 1.0f));

    ImGui::SetNextWindowPos(ImVec2(10.0f, 60.0f), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(380.0f, 0.0f), ImGuiCond_FirstUseEver);

    ImGui::Begin("Hooke's Law Spring");

    bool needsResetOnly = false; // structural changes (mode) don't need full rebuild

    ImGui::Text("Free particle integrator:");
    if (ImGui::RadioButton("Explicit Euler", &integrationModeIndex, 0))
    {
        freeParticle->setIntegrationMode(IntegrationMode::ExplicitEuler);
    }
    ImGui::SameLine();
    if (ImGui::RadioButton("Semi-Implicit Euler", &integrationModeIndex, 1))
    {
        freeParticle->setIntegrationMode(IntegrationMode::SemiImplicitEuler);
    }

    ImGui::Separator();

    if (ImGui::SliderFloat("Stiffness (k)", &stiffness, 1.0f, 400.0f))
    {
        spring->setStiffness(stiffness);
    }
    if (ImGui::SliderFloat("Rest length", &restLength, 0.2f, 4.0f))
    {
        spring->setRestLength(restLength);
    }
    if (ImGui::SliderFloat("Damping", &damping, 0.0f, 10.0f))
    {
        spring->setDamping(damping);
    }

    ImGui::Separator();

    // Stability estimate for the free particle treated as a simple
    // harmonic oscillator: omega = sqrt(k / m). Semi-implicit Euler
    // is bounded roughly for omega*dt < 2; explicit Euler grows for
    // ANY omega*dt > 0, however small.
    float omega = std::sqrt(stiffness / freeParticle->mass);
    float omegaDt = omega * lastFixedDt;

    ImGui::Text("omega * dt = %.3f", omegaDt);

    if (integrationModeIndex == 0)
    {
        ImGui::TextColored(
            ImVec4(1.0f, 0.6f, 0.2f, 1.0f),
            "Explicit Euler: unconditionally unstable for a spring — "
            "energy grows every step, however small dt is.");
    }
    else if (omegaDt >= 2.0f)
    {
        ImGui::TextColored(
            ImVec4(1.0f, 0.3f, 0.3f, 1.0f),
            "omega*dt >= 2 — expect this to blow up. Lower k or "
            "increase mass.");
    }
    else
    {
        ImGui::TextColored(
            ImVec4(0.3f, 1.0f, 0.4f, 1.0f),
            "omega*dt < 2 — semi-implicit Euler should stay bounded.");
    }

    ImGui::Separator();

    float speed = length(freeParticle->velocity);
    Vec2 delta = freeParticle->position - anchorParticle->position;
    float currentLength = length(delta);

    ImGui::Text("Speed: %.3f   Spring length: %.3f (rest %.3f)",
                speed, currentLength, restLength);
    ImGui::Text("Total energy: %.3f", computeTotalEnergy());

    ImGui::PlotLines(
        "Energy (KE + gravity PE + spring PE)",
        energyHistory,
        energyHistoryLength,
        energyHistoryOffset,
        nullptr,
        0.0f, FLT_MAX,
        ImVec2(0.0f, 60.0f));

    ImGui::Separator();

    if (ImGui::Button("Reset"))
    {
        reset();
    }

    ImGui::End();

    (void)needsResetOnly;
}