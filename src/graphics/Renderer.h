#pragma once

struct GLFWwindow;

#include "math/Vec2.h"

struct Color
{
    float r;
    float g;
    float b;
    float a;

    constexpr Color(
        float r,
        float g,
        float b,
        float a = 1.0f)
        : r(r), g(g), b(b), a(a)
    {
    }
};

class Renderer
{
public:
    // World-space convention:
    //
    //   1 world unit = 50 pixels
    //   world origin = center of the window
    //   +X = right
    //   +Y = up
    //
    // Screen coordinates use +Y downward, so Renderer
    // converts world Y to screen Y by flipping the sign.

    bool initialize(GLFWwindow *window);
    void shutdown();

    void beginFrame();
    void endFrame();

    void drawLine(
        Vec2 a,
        Vec2 b,
        Color color);

    void drawCircle(
        Vec2 center,
        float radius,
        Color color,
        int segments = 24);

    void drawBox(
        Vec2 center,
        Vec2 halfExtents,
        float angleRadians,
        Color color);

private:
    float worldToPixels = 50.0f;
};