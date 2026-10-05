#include "scenes/ClothScene.h"

#include <algorithm>
#include <cmath>
#include <memory>

#include <imgui.h>

#include "graphics/Renderer.h"
#include "physics/DragGenerator.h"
#include "physics/GravityGenerator.h"

namespace
{
    float length(Vec2 v)
    {
        return std::sqrt(v.x * v.x + v.y * v.y);
    }

    // Moves a Verlet particle without giving it velocity. Setting
    // oldPosition too is what makes the derived velocity
    // (position - oldPosition) / dt come out as zero.
    void teleport(VerletParticle &particle, Vec2 position)
    {
        particle.position = position;
        particle.oldPosition = position;
    }

    // Normal edge colour fading to red as a link departs from its rest
    // length (either direction). Fully red at 10% strain.
    Color strainColor(float strain)
    {
        float t = std::min(std::fabs(strain) / 0.10f, 1.0f);

        return Color(
            0.8f + 0.2f * t,
            0.7f - 0.5f * t,
            0.5f - 0.3f * t);
    }
}

void ClothScene::onEnter()
{
    rebuildCloth();
}

void ClothScene::onExit()
{
    release();
}

bool ClothScene::isPinnedCell(int row, int col) const
{
    if (row != 0)
    {
        return false;
    }

    return pinWholeTopRow || col == 0 || col == cols - 1;
}

Vec2 ClothScene::topLeft() const
{
    float halfWidth = 0.5f * segmentLength * static_cast<float>(cols - 1);
    return anchorCenter - Vec2(halfWidth, 0.0f);
}

void ClothScene::addLink(int a, int b, float restScale, bool isShear)
{
    VerletDistanceConstraint *constraint =
        world.addVerletConstraint(
            std::make_unique<VerletDistanceConstraint>(
                *clothParticles[a],
                *clothParticles[b],
                segmentLength * restScale));

    clothConstraints.push_back(constraint);
    links.push_back(Link{a, b, restScale, isShear});
}

void ClothScene::rebuildCloth()
{
    // Drop the grab before clear() destroys the particle it refers to.
    grabbedIndex = -1;
    hoveredIndex = -1;

    rows = uiRows;
    cols = uiCols;

    world.clear();
    clothParticles.clear();
    clothConstraints.clear();
    links.clear();
    pinnedFlags.assign(static_cast<std::size_t>(rows * cols), 0);

    world.addForceGenerator(
        std::make_unique<GravityGenerator>(Vec2(0.0f, -9.81f)));

    world.addForceGenerator(
        std::make_unique<DragGenerator>(0.15f));

    world.setConstraintIterations(constraintIterations);

    const Vec2 origin = topLeft();

    clothParticles.reserve(static_cast<std::size_t>(rows * cols));

    for (int row = 0; row < rows; ++row)
    {
        for (int col = 0; col < cols; ++col)
        {
            bool pinned = isPinnedCell(row, col);

            Vec2 position =
                origin + Vec2(
                             segmentLength * static_cast<float>(col),
                             -segmentLength * static_cast<float>(row));

            // mass 0 -> inverseMass 0 -> never moves under
            // integration or constraint solving.
            VerletParticle *particle = world.createVerletParticle(
                position,
                Vec2(0.0f, 0.0f),
                pinned ? 0.0f : particleMass,
                1.0f / 60.0f); // matches Application's fixed timestep

            clothParticles.push_back(particle);
            pinnedFlags[indexOf(row, col)] = pinned ? 1 : 0;
        }
    }

    // Structural links, added top-to-bottom so each Gauss-Seidel sweep
    // starts at the anchors and propagates corrections downward.
    for (int row = 0; row < rows; ++row)
    {
        for (int col = 0; col < cols; ++col)
        {
            int current = indexOf(row, col);

            if (col > 0)
            {
                addLink(indexOf(row, col - 1), current, 1.0f, false);
            }

            if (row > 0)
            {
                addLink(indexOf(row - 1, col), current, 1.0f, false);
            }
        }
    }

    // Shear links go in after all structural ones, so within each
    // iteration they solve last and don't fight the structure.
    if (useShear)
    {
        const float diagonal = std::sqrt(2.0f);

        for (int row = 0; row < rows - 1; ++row)
        {
            for (int col = 0; col < cols - 1; ++col)
            {
                addLink(
                    indexOf(row, col),
                    indexOf(row + 1, col + 1),
                    diagonal, true);

                addLink(
                    indexOf(row, col + 1),
                    indexOf(row + 1, col),
                    diagonal, true);
            }
        }
    }
}

void ClothScene::repinAnchors()
{
    const Vec2 origin = topLeft();

    for (int col = 0; col < cols; ++col)
    {
        if (pinnedFlags[col])
        {
            teleport(
                *clothParticles[col],
                origin + Vec2(segmentLength * static_cast<float>(col), 0.0f));
        }
    }
}

void ClothScene::applySegmentLength()
{
    for (std::size_t i = 0; i < clothConstraints.size(); ++i)
    {
        clothConstraints[i]->setTargetDistance(
            segmentLength * links[i].restScale);
    }

    // Targets changed, so the pinned corners must move too, otherwise
    // the top row stays at its old spacing and the whole cloth is
    // stretched or slackened against its own constraints.
    repinAnchors();
}

int ClothScene::findNearestFreeParticle(Vec2 point) const
{
    float pickRadius = std::max(0.2f, segmentLength * 0.6f);
    float bestDistanceSq = pickRadius * pickRadius;
    int best = -1;

    for (std::size_t i = 0; i < clothParticles.size(); ++i)
    {
        // Pinned particles aren't grabbable, so skip them and let the
        // nearest *movable* particle win instead.
        if (pinnedFlags[i])
        {
            continue;
        }

        Vec2 delta = clothParticles[i]->position - point;
        float distanceSq = delta.x * delta.x + delta.y * delta.y;

        if (distanceSq < bestDistanceSq)
        {
            bestDistanceSq = distanceSq;
            best = static_cast<int>(i);
        }
    }

    return best;
}

void ClothScene::grab(int index, Vec2 mousePosition)
{
    grabbedIndex = index;
    dragTarget = mousePosition;

    // Infinite mass while held: integrate() skips it and every
    // constraint gives it a zero correction share, so the rest of the
    // cloth has to adapt to wherever the mouse puts it.
    VerletParticle &particle = *clothParticles[index];
    grabbedInverseMass = particle.inverseMass;
    particle.inverseMass = 0.0f;
}

void ClothScene::release()
{
    if (grabbedIndex >= 0 &&
        grabbedIndex < static_cast<int>(clothParticles.size()))
    {
        VerletParticle &particle = *clothParticles[grabbedIndex];

        particle.inverseMass = grabbedInverseMass;

        // Zero velocity on release. oldPosition went stale while the
        // particle was static; leaving it would make the derived
        // velocity (position - oldPosition) / dt enormous.
        particle.oldPosition = particle.position;
    }

    grabbedIndex = -1;
}

void ClothScene::handleMouse(const Renderer &renderer)
{
    ImGuiIO &io = ImGui::GetIO();

    const bool mouseValid = ImGui::IsMousePosValid();

    Vec2 mouseWorld = dragTarget;

    if (mouseValid)
    {
        mouseWorld = renderer.screenToWorld(
            Vec2(io.MousePos.x, io.MousePos.y));
    }

    hoveredIndex = -1;

    // Mid-drag: keep following the mouse until the button comes up,
    // even if the cursor wanders over a UI window.
    if (grabbedIndex >= 0)
    {
        if (ImGui::IsMouseDown(ImGuiMouseButton_Left))
        {
            dragTarget = mouseWorld;
        }
        else
        {
            release();
        }

        return;
    }

    // Don't pick through the control panels.
    if (!mouseValid || io.WantCaptureMouse)
    {
        return;
    }

    hoveredIndex = findNearestFreeParticle(mouseWorld);

    if (hoveredIndex >= 0 && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
    {
        // Grab/release touch particle state between fixed steps, never
        // during one, so it's safe to do here in the render pass.
        grab(hoveredIndex, mouseWorld);
    }
}

void ClothScene::onUpdate(float fixedDt)
{
    if (grabbedIndex >= 0)
    {
        VerletParticle &particle = *clothParticles[grabbedIndex];

        Vec2 toTarget = dragTarget - particle.position;
        float distance = length(toTarget);
        float maxStep = maxDragSpeed * fixedDt;

        if (distance > maxStep)
        {
            toTarget = toTarget * (maxStep / distance);
        }

        // Applied before the step so the constraint solve sees the
        // new position and pulls the neighbours along this very frame.
        teleport(particle, particle.position + toTarget);
    }

    world.step(fixedDt);
}

void ClothScene::onRender(Renderer &renderer)
{
    if (clothParticles.empty())
    {
        return;
    }

    handleMouse(renderer);

    // Shear links first and dim, so they read as a faint backing
    // structure rather than competing with the real grid.
    for (const Link &link : links)
    {
        if (link.isShear)
        {
            renderer.drawLine(
                clothParticles[link.a]->position,
                clothParticles[link.b]->position,
                Color(0.2f, 0.28f, 0.4f));
        }
    }

    for (const Link &link : links)
    {
        if (link.isShear)
        {
            continue;
        }

        Vec2 a = clothParticles[link.a]->position;
        Vec2 b = clothParticles[link.b]->position;

        float rest = segmentLength * link.restScale;
        float strain = length(b - a) / rest - 1.0f;

        renderer.drawLine(a, b, strainColor(strain));
    }

    for (std::size_t i = 0; i < clothParticles.size(); ++i)
    {
        bool pinned = pinnedFlags[i] != 0;

        if (!pinned && !drawParticles)
        {
            continue;
        }

        renderer.drawCircle(
            clothParticles[i]->position,
            pinned ? 0.12f : 0.06f,
            pinned ? Color(1.0f, 1.0f, 1.0f) : Color(0.9f, 0.8f, 0.3f));
    }

    if (grabbedIndex >= 0)
    {
        Vec2 held = clothParticles[grabbedIndex]->position;

        // Line from the particle to the cursor makes the drag speed
        // cap visible: it stretches when the mouse outruns the particle.
        renderer.drawLine(held, dragTarget, Color(0.4f, 1.0f, 0.5f));
        renderer.drawCircle(held, 0.14f, Color(0.4f, 1.0f, 0.5f));
    }
    else if (hoveredIndex >= 0)
    {
        renderer.drawCircle(
            clothParticles[hoveredIndex]->position,
            0.12f,
            Color(1.0f, 1.0f, 0.4f));
    }

    // ------------------------------------------------------------
    // UI
    // ------------------------------------------------------------
    ImGui::SetNextWindowPos(ImVec2(10.0f, 60.0f), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(380.0f, 0.0f), ImGuiCond_FirstUseEver);

    ImGui::Begin("Cloth");

    ImGui::TextWrapped(
        "Left-drag any non-white particle to pull the cloth. Edges "
        "turn red where a link is stretched or squashed.");

    ImGui::Separator();

    if (ImGui::SliderInt(
            "Constraint iterations", &constraintIterations, 1, 20))
    {
        world.setConstraintIterations(constraintIterations);
    }

    if (ImGui::SliderFloat("Segment length", &segmentLength, 0.1f, 1.0f))
    {
        applySegmentLength();
    }

    bool anchorMoved = false;
    anchorMoved |= ImGui::SliderFloat("Anchor X", &anchorCenter.x, -4.0f, 4.0f);
    anchorMoved |= ImGui::SliderFloat("Anchor Y", &anchorCenter.y, -1.0f, 5.0f);

    if (anchorMoved)
    {
        repinAnchors();
    }

    ImGui::Checkbox("Draw particles", &drawParticles);

    ImGui::Separator();

    ImGui::Text("Structure (applies on rebuild)");
    ImGui::SliderInt("Rows", &uiRows, 3, 40);
    ImGui::SliderInt("Columns", &uiCols, 3, 60);
    ImGui::SliderFloat("Particle mass", &particleMass, 0.05f, 2.0f);

    // These change which constraints exist, so they rebuild at once
    // from the same flat starting pose - a fair A/B comparison.
    if (ImGui::Checkbox("Shear (diagonal) constraints", &useShear))
    {
        rebuildCloth();
    }

    if (ImGui::Checkbox("Pin whole top row", &pinWholeTopRow))
    {
        rebuildCloth();
    }

    if (ImGui::Button("Rebuild Cloth"))
    {
        rebuildCloth();
    }

    ImGui::Separator();

    // Stretch statistics over structural links only; shear links have
    // a different rest length and would muddy the number.
    float strainSum = 0.0f;
    float strainMax = 0.0f;
    int structuralCount = 0;

    for (const Link &link : links)
    {
        if (link.isShear)
        {
            continue;
        }

        Vec2 delta =
            clothParticles[link.b]->position -
            clothParticles[link.a]->position;

        float strain = length(delta) / segmentLength - 1.0f;

        strainSum += strain;
        strainMax = std::max(strainMax, strain);
        ++structuralCount;
    }

    float strainAverage =
        structuralCount > 0
            ? strainSum / static_cast<float>(structuralCount)
            : 0.0f;

    ImGui::Text("Particles: %d   Constraints: %d",
                static_cast<int>(clothParticles.size()),
                static_cast<int>(links.size()));

    ImGui::Text("Average stretch: %.2f%%", strainAverage * 100.0f);
    ImGui::Text("Worst link:      %.2f%%", strainMax * 100.0f);

    ImGui::End();
}