#include "scenes/BroadPhaseComparisonScene.h"

#include <random>

#include <imgui.h>

#include "graphics/Renderer.h"
#include "scenes/BodyDrawing.h"

namespace
{
    // Sparse layout world bounds (half extents). Larger than the window on
    // purpose: most bodies are far apart, which is where a grid wins.
    constexpr float WORLD_HALF_WIDTH = 40.0f;
    constexpr float WORLD_HALF_HEIGHT = 25.0f;

    // Cell sizes used to check correctness is independent of cell size.
    // Includes sizes far smaller and far larger than the bodies.
    constexpr float SWEEP_SIZES[] = {0.1f, 0.25f, 0.5f, 0.75f, 1.0f, 1.3f, 2.0f,
                                     3.7f, 5.0f, 8.0f, 13.0f, 50.0f, 200.0f};
}

void BroadPhaseComparisonScene::onEnter()
{
    // Re-entrant: this object outlives each visit to the scene.
    world.clear();
    bodies.clear();

    if (layout == Layout::Test)
    {
        buildTestLayout();
    }
    else
    {
        buildSparseLayout();
    }
}

void BroadPhaseComparisonScene::buildTestLayout()
{
    // Not overlapping (AABB x: [-1,1] vs [1.5,3.5]) but, at the default
    // cell size of 2, in the same cell. A grid without an AABB overlap
    // filter wrongly pairs these.
    bodies.push_back(world.addRigidBody(RigidBody::createCircle(Vec2(0.0f, 0.0f), 1.0f, 1.0f)));
    bodies.push_back(world.addRigidBody(RigidBody::createCircle(Vec2(2.5f, 0.0f), 1.0f, 1.0f)));

    // Far from everything.
    bodies.push_back(world.addRigidBody(RigidBody::createCircle(Vec2(10.0f, 10.0f), 0.5f, 1.0f)));

    // Two overlapping static bodies in negative coordinates: static-static
    // must be skipped and negative cells must floor correctly.
    bodies.push_back(world.addRigidBody(RigidBody::createCircle(Vec2(-5.0f, -5.0f), 0.5f, 0.0f)));
    bodies.push_back(world.addRigidBody(RigidBody::createCircle(Vec2(-5.5f, -5.5f), 0.5f, 0.0f)));

    // A dynamic body overlapping one of those statics: must be kept.
    bodies.push_back(world.addRigidBody(RigidBody::createCircle(Vec2(-5.2f, -4.4f), 0.5f, 1.0f)));

    // A big rotated box spanning many cells at small cell sizes. It shares
    // several cells with the same bodies, so one pair is reachable through
    // multiple cells (dedup must handle it).
    RigidBody *big = world.addRigidBody(RigidBody::createBox(Vec2(1.0f, 1.5f), Vec2(3.0f, 0.5f), 1.0f));
    big->rotation = 0.6f;
    bodies.push_back(big);

    // Exactly touching AABBs (edges coincide): counts as overlap.
    bodies.push_back(world.addRigidBody(RigidBody::createBox(Vec2(6.0f, -3.0f), Vec2(1.0f, 1.0f), 1.0f)));
    bodies.push_back(world.addRigidBody(RigidBody::createBox(Vec2(8.0f, -3.0f), Vec2(1.0f, 1.0f), 1.0f)));

    // Straddling the origin: lands in cells with both signs.
    bodies.push_back(world.addRigidBody(RigidBody::createCircle(Vec2(0.0f, -3.0f), 0.8f, 1.0f)));
    bodies.push_back(world.addRigidBody(RigidBody::createCircle(Vec2(0.5f, -3.5f), 0.8f, 1.0f)));
}

void BroadPhaseComparisonScene::buildSparseLayout()
{
    // Fixed seed: the same scene every visit, so numbers are comparable.
    std::mt19937 rng(12345);
    std::uniform_real_distribution<float> posX(-WORLD_HALF_WIDTH, WORLD_HALF_WIDTH);
    std::uniform_real_distribution<float> posY(-WORLD_HALF_HEIGHT, WORLD_HALF_HEIGHT);
    std::uniform_real_distribution<float> size(0.2f, 0.5f);
    std::uniform_real_distribution<float> speed(-2.0f, 2.0f);
    std::uniform_real_distribution<float> angle(0.0f, 3.14159f);
    std::uniform_int_distribution<int> kind(0, 9);

    for (int i = 0; i < sparseCount; ++i)
    {
        const Vec2 position(posX(rng), posY(rng));
        const float s = size(rng);
        const int k = kind(rng);

        // Mostly dynamic, ~10% static, mixed circles and boxes.
        const float mass = (k == 0) ? 0.0f : 1.0f;

        RigidBody *body = (k % 2 == 0)
                              ? world.addRigidBody(RigidBody::createCircle(position, s, mass))
                              : world.addRigidBody(RigidBody::createBox(position, Vec2(s, s * 0.6f), mass));
        body->rotation = angle(rng);
        if (!body->isStatic())
        {
            body->linearVelocity = Vec2(speed(rng), speed(rng));
        }
        bodies.push_back(body);
    }
}

void BroadPhaseComparisonScene::bounceInsideWorld()
{
    for (RigidBody *body : bodies)
    {
        if (body->isStatic())
        {
            continue;
        }
        if (body->position.x < -WORLD_HALF_WIDTH || body->position.x > WORLD_HALF_WIDTH)
        {
            body->linearVelocity.x = -body->linearVelocity.x;
            body->position.x = body->position.x < 0.0f ? -WORLD_HALF_WIDTH : WORLD_HALF_WIDTH;
        }
        if (body->position.y < -WORLD_HALF_HEIGHT || body->position.y > WORLD_HALF_HEIGHT)
        {
            body->linearVelocity.y = -body->linearVelocity.y;
            body->position.y = body->position.y < 0.0f ? -WORLD_HALF_HEIGHT : WORLD_HALF_HEIGHT;
        }
    }
}

void BroadPhaseComparisonScene::onUpdate(float fixedDt)
{
    world.step(fixedDt);

    if (layout == Layout::Sparse)
    {
        bounceInsideWorld();
    }

    const auto &all = world.getBodies();

    // Same bodies, both algorithms, current cell size.
    bruteStats = {};
    gridStats = {};
    const std::vector<BodyPair> brute = findPairsBruteForce(all, &bruteStats);
    const std::vector<BodyPair> grid = findPairsSpatialGrid(all, cellSize, &gridStats);

    brutePairCount = brute.size();
    gridPairCount = grid.size();
    const PairSet reference = makePairSet(brute);
    setsMatch = grid.size() == brute.size() && makePairSet(grid) == reference;

    // Correctness must not depend on cell size: check a spread of sizes.
    sweepSizesTested = 0;
    sweepSizesMatching = 0;
    firstMismatchingSize = 0.0f;
    for (const float size : SWEEP_SIZES)
    {
        const std::vector<BodyPair> sweep = findPairsSpatialGrid(all, size);
        ++sweepSizesTested;
        if (sweep.size() == brute.size() && makePairSet(sweep) == reference)
        {
            ++sweepSizesMatching;
        }
        else if (firstMismatchingSize == 0.0f)
        {
            firstMismatchingSize = size;
        }
    }
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

    // Highlight bodies in a candidate pair (the world's brute-force result),
    // so what the algorithms agree on is visible.
    for (const BodyPair &pair : world.getCandidatePairs())
    {
        drawAABB(renderer, pair.a->computeAABB(), Color(1.0f, 0.3f, 0.3f));
        drawAABB(renderer, pair.b->computeAABB(), Color(1.0f, 0.3f, 0.3f));
    }

    ImGui::SetNextWindowPos(ImVec2(10.0f, 10.0f), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(360.0f, 340.0f), ImGuiCond_FirstUseEver);

    ImGui::Begin("Grid Settings");

    int layoutIndex = (layout == Layout::Test) ? 0 : 1;
    bool layoutChanged = ImGui::RadioButton("Test scene", &layoutIndex, 0);
    ImGui::SameLine();
    layoutChanged |= ImGui::RadioButton("Sparse world", &layoutIndex, 1);
    if (layout == Layout::Sparse)
    {
        layoutChanged |= ImGui::SliderInt("Bodies", &sparseCount, 50, 1000);
    }
    if (layoutChanged)
    {
        layout = (layoutIndex == 0) ? Layout::Test : Layout::Sparse;
        onEnter();
    }

    ImGui::Checkbox("Draw Grid", &drawGrid);
    ImGui::SliderFloat("Cell Size", &cellSize, 0.25f, 20.0f);

    ImGui::Separator();
    ImGui::Text("Bodies: %zu", world.getBodies().size());
    ImGui::Text("Brute force: %zu pairs, %zu overlap tests",
                brutePairCount, bruteStats.overlapTests);
    ImGui::Text("Spatial grid: %zu pairs, %zu overlap tests",
                gridPairCount, gridStats.overlapTests);
    ImGui::Text("  (%zu cell insertions, %zu in-cell pair visits)",
                gridStats.cellInsertions, gridStats.cellPairVisits);

    if (gridStats.overlapTests > 0 && bruteStats.overlapTests > 0)
    {
        ImGui::Text("Grid does %.1f%% of brute-force tests (%.1fx fewer)",
                    100.0 * static_cast<double>(gridStats.overlapTests) / static_cast<double>(bruteStats.overlapTests),
                    static_cast<double>(bruteStats.overlapTests) / static_cast<double>(gridStats.overlapTests));
    }

    ImGui::Separator();
    if (setsMatch)
    {
        ImGui::TextColored(ImVec4(0.3f, 1.0f, 0.3f, 1.0f), "Pair sets match: YES");
    }
    else
    {
        ImGui::TextColored(ImVec4(1.0f, 0.3f, 0.3f, 1.0f), "Pair sets match: NO");
    }

    const bool sweepOk = (sweepSizesMatching == sweepSizesTested);
    ImGui::TextColored(
        sweepOk ? ImVec4(0.3f, 1.0f, 0.3f, 1.0f) : ImVec4(1.0f, 0.3f, 0.3f, 1.0f),
        "Cell-size sweep: %d/%d sizes match", sweepSizesMatching, sweepSizesTested);
    if (!sweepOk)
    {
        ImGui::Text("First mismatch at cell size %.2f", firstMismatchingSize);
    }

    ImGui::End();
}
