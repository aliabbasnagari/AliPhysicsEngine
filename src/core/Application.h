#pragma once

#include <memory>
#include <vector>

#include "core/Scene.h"
#include "graphics/Renderer.h"

struct GLFWwindow;

class Application
{
public:
    Application();
    ~Application();

    Application(const Application &) = delete;
    Application &operator=(const Application &) = delete;

    int run();

private:
    bool initialize();
    void shutdown();

    void registerScenes();

    void onUpdate(float fixedDt);
    void onRender();
    void renderMenu();

    bool hasActiveScene() const;

    GLFWwindow *window = nullptr;
    Renderer renderer;

    std::vector<std::unique_ptr<Scene>> scenes;
    int activeSceneIndex = -1;

    int width = 1280;
    int height = 720;
};