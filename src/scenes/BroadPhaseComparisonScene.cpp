#include "scenes/BroadPhaseComparisonScene.h"

#include <cstdio>
#include <unordered_set>
#include <cstdint>
#include <unordered_map>

#include <imgui.h>

#include "graphics/Renderer.h"
#include "scenes/BodyDrawing.h"

void BroadPhaseComparisonScene::onEnter()
{
    world.clear();
    bodies.clear();

    bodies.push_back(world.addRigidBody(RigidBody::createCircle(Vec2(0.0f, 0.0f), 1.0f, 1.0f)));
    bodies.push_back(world.addRigidBody(RigidBody::createCircle(Vec2(2.5f, 0.0f), 1.0f, 1.0f)));
    bodies.push_back(world.addRigidBody(RigidBody::createCircle(Vec2(10.0f, 10.0f), 0.5f, 1.0f)));
    bodies.push_back(world.addRigidBody(RigidBody::createCircle(Vec2(-5.0f, -5.0f), 0.5f, 0.0f)));
    bodies.push_back(world.addRigidBody(RigidBody::createCircle(Vec2(-5.5f, -5.5f), 0.5f, 0.0f)));

    auto bruteResult = findPairsBruteForce(world.getBodies());
    auto gridResult = findPairsSpatialGrid(world.getBodies(), cellSize);

    std::printf("Brute-force: %zu pairs\n", bruteResult.size());
    std::printf("Spatial grid: %zu pairs\n", gridResult.size());

    auto pairSet = [](const std::vector<BodyPair> &pairs)
    {
        std::unordered_set<size_t> set;
        for (const auto &p : pairs)
        {
            const RigidBody *a = p.a;
            const RigidBody *b = p.b;
            size_t key = std::min<uintptr_t>(reinterpret_cast<uintptr_t>(a), reinterpret_cast<uintptr_t>(b)) * 1000000 +
                         std::max<uintptr_t>(reinterpret_cast<uintptr_t>(a), reinterpret_cast<uintptr_t>(b));
            set.insert(key);
        }
        return set;
    };

    bool setsMatch = pairSet(bruteResult) == pairSet(gridResult);
    std::printf("Sets match: %s\n", setsMatch ? "YES" : "NO");
}

void BroadPhaseComparisonScene::onUpdate(float fixedDt)
{
    world.step(fixedDt);
}

void BroadPhaseComparisonScene::onRender(Renderer &renderer)
{
    if (drawGrid)
    {
        drawSpatialGrid(renderer, world.getBodies(), cellSize, Color(0.3f, 0.3f, 0.5f, 0.3f));
    }

    for (const auto &body : world.getBodies())
    {
        drawBody(renderer, *body);
    }

    ImGui::SetNextWindowPos(ImVec2(10.0f, 10.0f), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(200.0f, 100.0f), ImGuiCond_FirstUseEver);

    ImGui::Begin("Grid Settings");

    ImGui::Checkbox("Draw Grid", &drawGrid);
    ImGui::SliderFloat("Cell Size", &cellSize, 0.5f, 10.0f);

    ImGui::End();
}
