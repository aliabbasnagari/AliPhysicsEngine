#pragma once

#include <vector>

#include "core/Scene.h"
#include "math/Vec2.h"
#include "physics/DistanceConstraint.h"
#include "physics/PhysicsWorld.h"
#include "physics/VerletParticle.h"

class ClothScene : public Scene
{
public:
    const char *getName() const override { return "Cloth"; }

    void onEnter() override;
    void onExit() override;
    void onUpdate(float fixedDt) override;
    void onRender(Renderer &renderer) override;

private:
    // One entry per constraint, parallel to clothConstraints. Kept so
    // rendering and the stretch statistics can walk the links instead
    // of re-deriving the grid topology.
    struct Link
    {
        int a;
        int b;
        float restScale; // rest length = segmentLength * restScale
        bool isShear;
    };

    void rebuildCloth();
    void addLink(int a, int b, float restScale, bool isShear);
    void applySegmentLength();
    void repinAnchors();

    bool isPinnedCell(int row, int col) const;
    Vec2 topLeft() const;
    int indexOf(int row, int col) const { return row * cols + col; }

    void handleMouse(const Renderer &renderer);
    int findNearestFreeParticle(Vec2 point) const;
    void grab(int index, Vec2 mousePosition);
    void release();

    PhysicsWorld world;

    // Non-owning; the world owns everything.
    std::vector<VerletParticle *> clothParticles;
    std::vector<VerletDistanceConstraint *> clothConstraints;
    std::vector<Link> links;
    std::vector<char> pinnedFlags; // per particle; only row 0 is ever set

    // Built size (what clothParticles actually contains) vs. the
    // values the sliders are editing. They only sync on rebuild.
    int rows = 10;
    int cols = 10;
    int uiRows = 10;
    int uiCols = 10;

    float segmentLength = 0.3f;
    float particleMass = 0.2f;

    // Top-centre of the cloth. The pinned corners are placed relative
    // to this, so the cloth stays centred as size/segment length change.
    Vec2 anchorCenter = Vec2(0.0f, 3.0f);

    int constraintIterations = 8;

    bool useShear = false;
    bool pinWholeTopRow = false;
    bool drawParticles = true;

    // --- Mouse interaction ---
    int hoveredIndex = -1;
    int grabbedIndex = -1;
    float grabbedInverseMass = 0.0f;
    Vec2 dragTarget = Vec2(0.0f, 0.0f);

    // World units per second. Stops a fast flick of the mouse from
    // teleporting the grabbed particle several segments in one step.
    float maxDragSpeed = 20.0f;
};