#include "scenes/GravityScene.h"

#include <memory>

#include "graphics/Renderer.h"
#include "physics/GravityGenerator.h"

void GravityScene::onEnter()
{
    // Re-entrant: this object outlives each visit to the scene.
    world.clear();

    world.addForceGenerator(
        std::make_unique<GravityGenerator>(Vec2(0.0f, -9.81f)));

    world.createParticle(Vec2(-1.0f, 3.0f), Vec2(1.0f, 0.0f), 1.0f);
    world.createParticle(Vec2(0.0f, 3.0f), Vec2(0.0f, 0.0f), 2.0f);
    world.createParticle(Vec2(1.0f, 3.0f), Vec2(-1.0f, 0.0f), 0.5f);
}

void GravityScene::onUpdate(float fixedDt)
{
    world.step(fixedDt);
}

void GravityScene::onRender(Renderer &renderer)
{
    for (const auto &particle : world.getParticles())
    {
        renderer.drawCircle(
            particle->position,
            0.20f,
            Color(0.3f, 0.6f, 1.0f));
    }
}