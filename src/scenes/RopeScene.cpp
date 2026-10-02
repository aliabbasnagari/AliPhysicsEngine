#include "scenes/RopeScene.h"

#include <cmath>
#include <memory>

#include <imgui.h>

#include "graphics/Renderer.h"

void RopeScene::onEnter()
{
    rebuildRope();
}

void RopeScene::rebuildRope()
{
    world.clear();
    ropeParticles.clear();
    ropeConstraints.clear();

    world.addForceGenerator(
        std::make_unique<GravityGenerator>(Vec2(0.0f, -9.81f)));

    // A touch of drag so the rope settles rather than swinging
    // forever - Verlet conserves energy closely enough that without
    // this it genuinely would keep going. Easy to remove if you want
    // to see the undamped behavior instead.
    world.addForceGenerator(std::make_unique<DragGenerator>(0.15f));

    world.setConstraintIterations(constraintIterations);

    for (int i = 0; i < particleCount; ++i)
    {
        Vec2 position =
            anchorPosition + Vec2(0.0f, -segmentLength * static_cast<float>(i));

        // Particle 0 is the anchor: mass 0 -> inverseMass 0 ->
        // infinite mass, per VerletParticle's own convention. It
        // never moves under integration or constraint solving -
        // only our explicit UI code below ever touches its position.
        float mass = (i == 0) ? 0.0f : particleMass;

        VerletParticle *particle = world.createVerletParticle(
            position, Vec2(0.0f, 0.0f), mass, 1.0f / 60.0f);

        ropeParticles.push_back(particle);

        if (i > 0)
        {
            VerletDistanceConstraint *constraint =
                world.addVerletConstraint(
                    std::make_unique<VerletDistanceConstraint>(
                        *ropeParticles[i - 1], *particle, segmentLength));

            ropeConstraints.push_back(constraint);
        }
    }
}

void RopeScene::onUpdate(float fixedDt)
{
    world.step(fixedDt);
}

void RopeScene::onRender(Renderer &renderer)
{
    for (std::size_t i = 1; i < ropeParticles.size(); ++i)
    {
        renderer.drawLine(
            ropeParticles[i - 1]->position,
            ropeParticles[i]->position,
            Color(0.8f, 0.7f, 0.5f));
    }

    for (std::size_t i = 0; i < ropeParticles.size(); ++i)
    {
        bool isAnchor = (i == 0);

        renderer.drawCircle(
            ropeParticles[i]->position,
            isAnchor ? 0.12f : 0.08f,
            isAnchor ? Color(1.0f, 1.0f, 1.0f) : Color(0.9f, 0.8f, 0.3f));
    }

    ImGui::SetNextWindowPos(ImVec2(10.0f, 60.0f), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(380.0f, 0.0f), ImGuiCond_FirstUseEver);

    ImGui::Begin("Rope");

    // Iterations apply live - no rebuild needed. This is the single
    // most important slider in this scene.
    if (ImGui::SliderInt("Constraint iterations", &constraintIterations, 1, 20))
    {
        world.setConstraintIterations(constraintIterations);
    }

    // Segment length also applies live, pushed straight into every
    // existing constraint's target distance.
    if (ImGui::SliderFloat("Segment length", &segmentLength, 0.1f, 1.0f))
    {
        for (auto *constraint : ropeConstraints)
        {
            constraint->setTargetDistance(segmentLength);
        }
    }

    ImGui::Separator();

    // Structural changes - these only take effect on rebuild, since
    // they change array sizes / initial layout.
    ImGui::SliderInt("Particle count", &particleCount, 3, 40);
    ImGui::SliderFloat("Particle mass", &particleMass, 0.05f, 2.0f);

    if (ImGui::Button("Rebuild Rope"))
    {
        rebuildRope();
    }

    ImGui::Separator();

    // Grabbing the anchor: move it live and watch the chain follow.
    bool anchorMoved = false;
    anchorMoved |= ImGui::SliderFloat("Anchor X", &anchorPosition.x, -4.0f, 4.0f);
    anchorMoved |= ImGui::SliderFloat("Anchor Y", &anchorPosition.y, -1.0f, 5.0f);

    if (anchorMoved && !ropeParticles.empty())
    {
        // The anchor's inverseMass is 0, so integrate() never
        // touches its position - this assignment is the ONLY thing
        // that moves it, exactly like a hand holding the rope.
        ropeParticles.front()->position = anchorPosition;
    }

    // Perturbing a particle instead of the anchor - nudges the free
    // end and lets the disturbance travel back up the chain.
    if (ImGui::Button("Perturb free end") && ropeParticles.size() > 1)
    {
        ropeParticles.back()->position.x += 0.6f;
    }

    ImGui::Separator();

    // Numeric readout for the over-stretch criterion: sum of actual
    // segment lengths vs. the rest length they're all targeting.
    float restTotalLength =
        segmentLength * static_cast<float>(
                            ropeParticles.empty() ? 0 : ropeParticles.size() - 1);

    float actualTotalLength = 0.0f;
    for (std::size_t i = 1; i < ropeParticles.size(); ++i)
    {
        Vec2 delta = ropeParticles[i]->position - ropeParticles[i - 1]->position;
        actualTotalLength += std::sqrt(delta.x * delta.x + delta.y * delta.y);
    }

    float stretchPercent =
        (restTotalLength > 1e-6f)
            ? (actualTotalLength / restTotalLength - 1.0f) * 100.0f
            : 0.0f;

    ImGui::Text("Rest length:   %.3f", restTotalLength);
    ImGui::Text("Actual length: %.3f", actualTotalLength);
    ImGui::Text("Stretch: %.1f%%", stretchPercent);

    ImGui::End();
}