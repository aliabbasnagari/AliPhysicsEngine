#pragma once

class Renderer;

class Scene
{
public:
    virtual ~Scene() = default;

    // Menu label. Must be stable for the lifetime of the scene.
    virtual const char *getName() const = 0;

    // Called on every selection from the menu, so it must rebuild
    // state from scratch rather than append to it.
    virtual void onEnter() {}
    virtual void onExit() {}

    virtual void onUpdate(float fixedDt) {}
    virtual void onRender(Renderer &renderer) {}
};