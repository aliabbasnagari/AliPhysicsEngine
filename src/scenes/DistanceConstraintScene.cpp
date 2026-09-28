#include "scenes/DistanceConstraintScene.h"

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

void DistanceConstraintScene::onEnter()
{
    reset();
}

void DistanceConstraintScene::reset()
{
    // --- Pendulum comparison world ---
    pendulumWorld.clear();

    pendulumWorld.addForceGenerator(
        std::make_unique<GravityGenerator>(Vec2(0.0f, -9.81f)));

    constraintAnchor = pendulumWorld.createParticle(
        constraintAnchorPosition, Vec2(0.0f, 0.0f), 0.0f); // invMass 0
    constraintAnchorStartPosition = constraintAnchor->position;

    constraintFree = pendulumWorld.createParticle(
        constraintAnchorPosition + Vec2(pendulumLength, 0.0f),
        Vec2(0.0f, 0.0f),
        1.0f);

    pendulumConstraint = pendulumWorld.addConstraint(
        std::make_unique<DistanceConstraint>(
            *constraintAnchor, *constraintFree, pendulumLength));

    springAnchor = pendulumWorld.createParticle(
        springAnchorPosition, Vec2(0.0f, 0.0f), 0.0f);

    springFree = pendulumWorld.createParticle(
        springAnchorPosition + Vec2(pendulumLength, 0.0f),
        Vec2(0.0f, 0.0f),
        1.0f);

    pendulumSpring = pendulumWorld.addSpring(
        std::make_unique<SpringForceGenerator>(
            *springAnchor, *springFree,
            springStiffness, pendulumLength, springDamping));

    constraintTrail.clear();
    constraintTrail.push_back(constraintFree->position);
    springTrail.clear();
    springTrail.push_back(springFree->position);

    for (int i = 0; i < historyLength; ++i)
    {
        constraintDistanceHistory[i] = pendulumLength;
        springDistanceHistory[i] = pendulumLength;
    }
    historyOffset = 0;

    // --- Free-free world ---
    freeFreeWorld.clear();

    // Deliberately no gravity generator here. With zero initial
    // velocity and zero external force, integrate() leaves both
    // particles exactly where they started - so any position change
    // we observe this frame can only have come from the constraint
    // solve. Nothing else in this world is capable of moving them.
    freeA = freeFreeWorld.createParticle(
        freeAStartPosition, Vec2(0.0f, 0.0f), freeMass);
    freeB = freeFreeWorld.createParticle(
        freeBStartPosition, Vec2(0.0f, 0.0f), freeMass);

    freeFreeConstraint = freeFreeWorld.addConstraint(
        std::make_unique<DistanceConstraint>(
            *freeA, *freeB, freeFreeTargetDistance));

    lastFreeADelta = 0.0f;
    lastFreeBDelta = 0.0f;
}

void DistanceConstraintScene::onUpdate(float fixedDt)
{
    pendulumWorld.step(fixedDt);

    constraintTrail.push_back(constraintFree->position);
    if (constraintTrail.size() > trailLength)
        constraintTrail.pop_front();

    springTrail.push_back(springFree->position);
    if (springTrail.size() > trailLength)
        springTrail.pop_front();

    constraintDistanceHistory[historyOffset] =
        length(constraintFree->position - constraintAnchor->position);

    springDistanceHistory[historyOffset] =
        length(springFree->position - springAnchor->position);

    historyOffset = (historyOffset + 1) % historyLength;

    // --- Free-free: snapshot before, diff after. Isolates exactly
    // what the constraint solve did this frame. ---
    Vec2 beforeA = freeA->position;
    Vec2 beforeB = freeB->position;

    freeFreeWorld.step(fixedDt);

    lastFreeADelta = length(freeA->position - beforeA);
    lastFreeBDelta = length(freeB->position - beforeB);
}

void DistanceConstraintScene::drawTrail(
    Renderer &renderer,
    const std::deque<Vec2> &trailPoints,
    const Color &color) const
{
    for (std::size_t i = 1; i < trailPoints.size(); ++i)
    {
        renderer.drawLine(trailPoints[i - 1], trailPoints[i], color);
    }
}

void DistanceConstraintScene::onRender(Renderer &renderer)
{
    Color constraintColor(0.3f, 0.8f, 1.0f); // blue - rigid
    Color springColor(1.0f, 0.6f, 0.2f);     // orange - soft
    Color dimConstraint(0.15f, 0.3f, 0.4f);
    Color dimSpring(0.4f, 0.25f, 0.1f);

    // Pendulum comparison.
    drawTrail(renderer, constraintTrail, dimConstraint);
    drawTrail(renderer, springTrail, dimSpring);

    renderer.drawLine(
        constraintAnchor->position, constraintFree->position, constraintColor);
    renderer.drawCircle(constraintAnchor->position, 0.10f, Color(1, 1, 1));
    renderer.drawCircle(constraintFree->position, 0.16f, constraintColor);

    renderer.drawLine(
        springAnchor->position, springFree->position, springColor);
    renderer.drawCircle(springAnchor->position, 0.10f, Color(1, 1, 1));
    renderer.drawCircle(springFree->position, 0.16f, springColor);

    // Free-free pair.
    renderer.drawLine(freeA->position, freeB->position, Color(0.5f, 1.0f, 0.5f));
    renderer.drawCircle(freeA->position, 0.14f, Color(0.4f, 0.9f, 0.5f));
    renderer.drawCircle(freeB->position, 0.14f, Color(0.4f, 0.9f, 0.5f));

    ImGui::SetNextWindowPos(ImVec2(10.0f, 60.0f), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(420.0f, 0.0f), ImGuiCond_FirstUseEver);

    ImGui::Begin("Distance Constraint");

    ImGui::TextColored(ImVec4(0.3f, 0.8f, 1.0f, 1.0f), "blue = constraint (rigid)");
    ImGui::SameLine();
    ImGui::TextColored(ImVec4(1.0f, 0.6f, 0.2f, 1.0f), "  orange = spring (soft)");

    ImGui::Separator();

    if (ImGui::SliderFloat("Pendulum length", &pendulumLength, 0.5f, 3.0f))
    {
        pendulumConstraint->setTargetDistance(pendulumLength);
        pendulumSpring->setRestLength(pendulumLength);
    }
    if (ImGui::SliderFloat("Spring stiffness (k)", &springStiffness, 1.0f, 200.0f))
    {
        pendulumSpring->setStiffness(springStiffness);
    }
    if (ImGui::SliderFloat("Spring damping", &springDamping, 0.0f, 5.0f))
    {
        pendulumSpring->setDamping(springDamping);
    }

    float currentConstraintDistance =
        length(constraintFree->position - constraintAnchor->position);
    float currentSpringDistance =
        length(springFree->position - springAnchor->position);

    ImGui::Text(
        "Constraint length: %.4f (target %.4f)",
        currentConstraintDistance, pendulumLength);
    ImGui::Text(
        "Spring length:     %.4f (rest   %.4f)",
        currentSpringDistance, pendulumLength);

    ImGui::PlotLines(
        "Constraint length", constraintDistanceHistory, historyLength,
        historyOffset, nullptr, 0.0f, FLT_MAX, ImVec2(0.0f, 50.0f));
    ImGui::PlotLines(
        "Spring length", springDistanceHistory, historyLength,
        historyOffset, nullptr, 0.0f, FLT_MAX, ImVec2(0.0f, 50.0f));

    ImGui::Separator();

    float anchorDrift =
        length(constraintAnchor->position - constraintAnchorStartPosition);

    ImGui::Text(
        "Anchor (inverseMass=0) drift from start: %.6f %s",
        anchorDrift,
        anchorDrift < 1e-5f ? "(never moved)" : "(!!! moved)");

    ImGui::Separator();

    ImGui::TextWrapped(
        "Free-free pair: equal mass, no gravity, zero initial velocity. "
        "Any movement below comes ONLY from the constraint solve.");

    if (ImGui::SliderFloat("Free-free target distance", &freeFreeTargetDistance, 0.3f, 3.0f))
    {
        freeFreeConstraint->setTargetDistance(freeFreeTargetDistance);
    }

    float currentFreeFreeDistance = length(freeB->position - freeA->position);

    ImGui::Text("Current separation: %.4f", currentFreeFreeDistance);
    ImGui::Text("Particle A moved this frame: %.6f", lastFreeADelta);
    ImGui::Text("Particle B moved this frame: %.6f", lastFreeBDelta);

    float splitDifference = std::fabs(lastFreeADelta - lastFreeBDelta);
    ImGui::Text(
        "Difference: %.6f %s",
        splitDifference,
        splitDifference < 1e-5f ? "(50/50 split confirmed)" : "(!!! not equal)");

    ImGui::Separator();

    if (ImGui::Button("Reset"))
    {
        reset();
    }

    ImGui::End();
}