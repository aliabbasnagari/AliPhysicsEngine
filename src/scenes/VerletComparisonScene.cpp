#include "scenes/VerletComparisonScene.h"

#include <cfloat>
#include <cmath>
#include <memory>

#include <imgui.h>

#include "graphics/Renderer.h"
#include "physics/GravityGenerator.h"

namespace
{
    float length(Vec2 v)
    {
        return std::sqrt(v.x * v.x + v.y * v.y);
    }
}

void VerletComparisonScene::onEnter()
{
    world.clear();
    semiImplicitParticle = nullptr;
    verletParticle = nullptr;

    semiImplicitTrail.clear();
    verletTrail.clear();

    for (int i = 0; i < historyLength; ++i)
    {
        positionErrorHistory[i] = 0.0f;
        velocityErrorHistory[i] = 0.0f;
    }
    historyOffset = 0;

    // Gravity only — no drag, no spring — so any divergence between
    // the two particles comes purely from the integrator, not from
    // some other force being applied inconsistently.
    world.addForceGenerator(
        std::make_unique<GravityGenerator>(Vec2(0.0f, -9.81f)));

    semiImplicitParticle =
        world.createParticle(startPosition, startVelocity, 1.0f);
    semiImplicitParticle->setIntegrationMode(
        IntegrationMode::SemiImplicitEuler);

    // VerletParticle's ctor takes an initial velocity purely to seed
    // oldPosition = position - velocity * fixedDt. It has no velocity
    // field of its own after this point — starting it here with the
    // same position/velocity as the Euler particle is what makes the
    // two trajectories comparable from frame zero.
    verletParticle = world.createVerletParticle(
        startPosition, startVelocity, 1.0f, lastFixedDt);
}

void VerletComparisonScene::onUpdate(float fixedDt)
{
    lastFixedDt = fixedDt;

    world.step(fixedDt);

    semiImplicitTrail.push_back(semiImplicitParticle->position);
    if (semiImplicitTrail.size() > trailLength)
    {
        semiImplicitTrail.pop_front();
    }

    verletTrail.push_back(verletParticle->position);
    if (verletTrail.size() > trailLength)
    {
        verletTrail.pop_front();
    }

    Vec2 positionDelta =
        semiImplicitParticle->position - verletParticle->position;

    // The only way to know a Verlet particle's speed is to ask it to
    // reconstruct one from where it was last step. There is no
    // stored velocity to read directly.
    Vec2 verletEstimatedVelocity = verletParticle->getVelocity(fixedDt);

    Vec2 velocityDelta =
        semiImplicitParticle->velocity - verletEstimatedVelocity;

    positionErrorHistory[historyOffset] = length(positionDelta);
    velocityErrorHistory[historyOffset] = length(velocityDelta);

    historyOffset = (historyOffset + 1) % historyLength;
}

void VerletComparisonScene::drawTrail(
    Renderer &renderer,
    const std::deque<Vec2> &trail,
    const Color &color) const
{
    for (std::size_t i = 1; i < trail.size(); ++i)
    {
        renderer.drawLine(trail[i - 1], trail[i], color);
    }
}

void VerletComparisonScene::onRender(Renderer &renderer)
{
    Color semiImplicitColor(0.3f, 0.6f, 1.0f); // blue
    Color verletColor(1.0f, 0.7f, 0.2f);       // orange
    Color dimSemiImplicit(0.15f, 0.25f, 0.4f);
    Color dimVerlet(0.4f, 0.3f, 0.1f);

    drawTrail(renderer, semiImplicitTrail, dimSemiImplicit);
    drawTrail(renderer, verletTrail, dimVerlet);

    // Reference line for the constraint-snap demo.
    renderer.drawLine(
        Vec2(-4.0f, snapTargetY),
        Vec2(4.0f, snapTargetY),
        Color(0.5f, 0.5f, 0.5f));

    renderer.drawCircle(
        semiImplicitParticle->position, 0.18f, semiImplicitColor);

    renderer.drawCircle(
        verletParticle->position, 0.18f, verletColor);

    ImGui::SetNextWindowPos(ImVec2(10.0f, 60.0f), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(380.0f, 0.0f), ImGuiCond_FirstUseEver);

    ImGui::Begin("Verlet vs Semi-Implicit Euler");

    Vec2 positionDelta =
        semiImplicitParticle->position - verletParticle->position;

    Vec2 verletEstimatedVelocity =
        verletParticle->getVelocity(lastFixedDt);

    Vec2 velocityDelta =
        semiImplicitParticle->velocity - verletEstimatedVelocity;

    ImGui::TextColored(
        ImVec4(0.3f, 0.6f, 1.0f, 1.0f),
        "Semi-Implicit  pos (%.2f, %.2f)  vel (%.2f, %.2f)",
        semiImplicitParticle->position.x,
        semiImplicitParticle->position.y,
        semiImplicitParticle->velocity.x,
        semiImplicitParticle->velocity.y);

    ImGui::TextColored(
        ImVec4(1.0f, 0.7f, 0.2f, 1.0f),
        "Verlet         pos (%.2f, %.2f)  vel* (%.2f, %.2f)",
        verletParticle->position.x,
        verletParticle->position.y,
        verletEstimatedVelocity.x,
        verletEstimatedVelocity.y);

    ImGui::TextDisabled("* reconstructed from position history, not stored");

    ImGui::Separator();

    ImGui::Text("Position error: %.4f", length(positionDelta));
    ImGui::Text("Velocity error: %.4f", length(velocityDelta));

    ImGui::PlotLines(
        "Pos error",
        positionErrorHistory,
        historyLength,
        historyOffset,
        nullptr,
        0.0f, FLT_MAX,
        ImVec2(0.0f, 50.0f));

    ImGui::PlotLines(
        "Vel error",
        velocityErrorHistory,
        historyLength,
        historyOffset,
        nullptr,
        0.0f, FLT_MAX,
        ImVec2(0.0f, 50.0f));

    ImGui::Separator();
    ImGui::TextWrapped(
        "Constraint demo: teleport both particles' position directly, "
        "as a constraint solver would, without touching velocity.");

    if (ImGui::Button("Snap Verlet to line"))
    {
        // This is the entire pitch for Verlet in constraint solving:
        // we only ever touch `position`. We do NOT touch oldPosition.
        // Next integrate() computes 2*position - oldPosition, and
        // because oldPosition still holds the pre-snap value, the
        // resulting "velocity" (position - oldPosition) automatically
        // reflects the teleport — no separate bookkeeping required.
        verletParticle->position.y = snapTargetY;
    }

    ImGui::SameLine();

    if (ImGui::Button("Snap Semi-Implicit to line"))
    {
        // Contrast: teleporting position here does nothing to
        // `velocity`. The particle is now in a new place but its
        // stored velocity still describes its old motion — anyone
        // reading `velocity` next frame gets a stale answer unless
        // this call site also patches it by hand. That extra manual
        // step, multiplied across every constraint in a solver, is
        // exactly what Verlet avoids.
        semiImplicitParticle->position.y = snapTargetY;
    }

    if (ImGui::Button("Reset"))
    {
        onEnter();
    }

    ImGui::End();
}