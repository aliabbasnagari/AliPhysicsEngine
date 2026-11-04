#pragma once

#include <vector>

#include "core/Scene.h"
#include "physics/PhysicsWorld.h"

// Runs brute force and the spatial grid on the same bodies every frame and
// shows whether the candidate pair sets match, plus how much work each did.
class BroadPhaseComparisonScene : public Scene
{
public:
    const char *getName() const override { return "BroadPhase Comparison"; }

    void onEnter() override;
    void onUpdate(float fixedDt) override;
    void onRender(Renderer &renderer) override;

private:
    enum class Layout
    {
        Test,  // A few hand-placed bodies covering the tricky cases.
        Sparse // Hundreds of bodies drifting around a large world.
    };

    void buildTestLayout();
    void buildSparseLayout();
    void bounceInsideWorld();

    PhysicsWorld world;
    std::vector<RigidBody *> bodies;

    Layout layout = Layout::Test;
    int sparseCount = 300;
    bool drawGrid = true;
    float cellSize = 2.0f;

    // Updated every frame from the current bodies.
    BroadPhaseStats bruteStats;
    BroadPhaseStats gridStats;
    size_t brutePairCount = 0;
    size_t gridPairCount = 0;
    bool setsMatch = true;

    // Sweep over many cell sizes on the current bodies.
    int sweepSizesTested = 0;
    int sweepSizesMatching = 0;
    float firstMismatchingSize = 0.0f;
};
