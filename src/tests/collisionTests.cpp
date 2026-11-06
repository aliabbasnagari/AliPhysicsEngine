#include <cmath>
#include <iostream>

#include "physics/Collision.h"

using namespace std;

static int failures = 0;

static bool near(float a, float b, float eps = 1e-4f)
{
    return fabs(a - b) < eps;
}

static void report(const char *name, bool ok)
{
    cout << name << " ........ " << (ok ? "PASS" : "FAIL") << endl;
    if (!ok)
        ++failures;
}

// Task 7-1: circle vs circle
void runTest71()
{
    // Acceptance 1: distance > sum of radii -> no collision
    report("7-1.1 separated circles: no collision",
           !collideCircles({0, 0}, 1.0f, {3, 0}, 1.0f).has_value());

    // Exactly touching (distance == sum) is NOT a collision
    report("7-1.1b exactly touching: no collision",
           !collideCircles({0, 0}, 1.0f, {2, 0}, 1.0f).has_value());

    // Acceptance 2a: on-axis overlap. distance 1.5, sum 2 -> depth 0.5,
    // normal (1,0), surface points (1,0) and (0.5,0) -> contact (0.75,0)
    {
        auto m = collideCircles({0, 0}, 1.0f, {1.5f, 0}, 1.0f);
        report("7-1.2a on-axis overlap: depth/normal/contact",
               m && near(m->depth, 0.5f) && m->normal == Vec2(1, 0) &&
                   m->pointCount == 1 && m->points[0] == Vec2(0.75f, 0));
    }

    // Acceptance 2b: 3-4-5 diagonal, radii 3 and 3. distance 5, sum 6
    // -> depth 1, normal (0.6, 0.8)
    {
        auto m = collideCircles({0, 0}, 3.0f, {3, 4}, 3.0f);
        report("7-1.2b diagonal 3-4-5: depth/normal",
               m && near(m->depth, 1.0f) && m->normal == Vec2(0.6f, 0.8f) &&
                   near(m->normal.length(), 1.0f));
    }

    // Unequal radii: A r=2 at origin, B r=1 at (2,0). distance 2, sum 3
    // -> depth 1. Surface points (2,0) and (1,0) -> contact (1.5, 0)
    {
        auto m = collideCircles({0, 0}, 2.0f, {2, 0}, 1.0f);
        report("7-1.2c unequal radii: depth/contact",
               m && near(m->depth, 1.0f) && m->points[0] == Vec2(1.5f, 0));
    }

    // Normal points A -> B regardless of side
    {
        auto m = collideCircles({0, 0}, 1.0f, {-1.5f, 0}, 1.0f);
        report("7-1.2d normal points A->B (B on the left)",
               m && m->normal == Vec2(-1, 0));
    }

    // Acceptance 3: concentric -> no crash, unit fallback normal,
    // depth = rA + rB
    {
        auto m = collideCircles({2, 2}, 1.0f, {2, 2}, 0.5f);
        report("7-1.3 concentric: fallback unit normal, depth = rA+rB",
               m && near(m->normal.length(), 1.0f) && near(m->depth, 1.5f) &&
                   m->normal == Vec2(1, 0));
    }

    // RigidBody wrapper agrees with the raw-value overload
    {
        RigidBody a = RigidBody::createCircle({0, 0}, 1.0f, 1.0f);
        RigidBody b = RigidBody::createCircle({1.5f, 0}, 1.0f, 1.0f);
        auto m = collideCircles(a, b);
        report("7-1.4 RigidBody wrapper",
               m && near(m->depth, 0.5f) && m->normal == Vec2(1, 0));
    }
}

int runCollisionTests()
{
    runTest71();

    cout << (failures == 0 ? "ALL PASSED" : "SOME TESTS FAILED") << endl;
    return failures == 0 ? 0 : 1;
}
