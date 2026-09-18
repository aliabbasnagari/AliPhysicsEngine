#pragma once

#include "graphics/Renderer.h"

struct GLFWwindow;

class Application
{
public:
    Application();
    virtual ~Application();

    int run();

protected:
    virtual void onUpdate(float fixedDt);
    virtual void onRender();

private:
    bool initialize();
    void shutdown();

    GLFWwindow *window = nullptr;
    Renderer renderer;

    int width = 1280;
    int height = 720;
};