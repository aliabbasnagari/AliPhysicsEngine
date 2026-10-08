#include "scenes/RigidBodyScene.h"

#include <memory>

#include "graphics/Renderer.h"
#include "scenes/BodyDrawing.h"

namespace
{
    constexpr float ARROW_SCALE = 2.0f; // world units per newton
    constexpr float RESET_SECONDS = 6.0f;

    // Applies the scene's loads through the world's normal force pass,
    // so they land after the accumulators are cleared and before
    // integration. Same pattern as GravityGenerator.
    class LoadGenerator : public ForceGenerator
    {
    public:
        explicit LoadGenerator(const std::vector<BodyLoad> &loads)
            : loads(loads)
        {
        }

        void updateForce(IForceReceiver &receiver, float /*dt*/) override
        {
            for (const BodyLoad &load : loads)
            {
                if (static_cast<IForceReceiver *>(load.body) != &receiver)
                {
                    continue;
                }
                if (load.force.lengthSquared() > 0.0f)
                {
                    // Application point must be in world space.
                    load.body->applyForceAtPoint(
                        load.force,
                        load.body->getTransform().localToWorld(load.localPoint));
                }
                if (load.torque != 0.0f)
                {
                    load.body->applyTorque(load.torque);
                }
            }
        }

    private:
        const std::vector<BodyLoad> &loads;
    };
}

void RigidBodyScene::addTest(
    RigidBody body, Vec2 force, Vec2 localPoint, float torque)
{
    RigidBody *added = world.addRigidBody(std::move(body));
    loads.push_back(BodyLoad{added, force, localPoint, torque});
}

void RigidBodyScene::onEnter()
{
    // Re-entrant: this object outlives each visit to the scene.
    world.clear();
    loads.clear();
    elapsed = 0.0f;

    // No gravity here, so the effect of each load is easy to see.
    world.addForceGenerator(std::make_unique<LoadGenerator>(loads));

    const Vec2 half(0.6f, 0.3f);
    const Vec2 up(0.0f, 0.3f);
    const Vec2 none(0.0f, 0.0f);
    const float y = -3.0f;

    // 1. Force through the centre of mass: translates, never spins.
    addTest(RigidBody::createBox(Vec2(-7.5f, y), half, 1.0f), up, none, 0.0f);

    // 2. Same force at the right edge: translates and spins.
    addTest(RigidBody::createBox(Vec2(-5.0f, y), half, 1.0f), up, Vec2(half.x, 0.0f), 0.0f);

    // 3. Mirrored point: spin direction reverses.
    addTest(RigidBody::createBox(Vec2(-2.5f, y), half, 1.0f), up, Vec2(-half.x, 0.0f), 0.0f);

    // 4. Torque only: spins in place.
    addTest(RigidBody::createBox(Vec2(0.0f, y), half, 1.0f), none, none, 0.3f);

    // 5. Static body under force and torque: must not move or rotate.
    RigidBody staticBox = RigidBody::createBox(Vec2(2.5f, y), half, 0.0f);
    staticBox.rotation = 0.5f;
    addTest(staticBox, up, Vec2(half.x, 0.0f), 0.3f);

    // 6. Equal mass and torque, radius doubled: the big one spins up slower.
    addTest(RigidBody::createCircle(Vec2(5.0f, y), 0.5f, 1.0f), none, none, 0.3f);
    addTest(RigidBody::createCircle(Vec2(7.5f, y), 1.0f, 1.0f), none, none, 0.3f);
}

void RigidBodyScene::onUpdate(float fixedDt)
{
    world.step(fixedDt);

    // Bodies drift off screen with no gravity or walls; restart the demo.
    elapsed += fixedDt;
    if (elapsed > RESET_SECONDS)
    {
        onEnter();
    }
}

void RigidBodyScene::onRender(Renderer &renderer)
{
    for (const auto &body : world.getBodies())
    {
        drawBody(renderer, *body);
    }

    // Force arrows from each application point.
    for (const BodyLoad &load : loads)
    {
        if (load.force.lengthSquared() <= 0.0f)
        {
            continue;
        }
        const Vec2 from = load.body->getTransform().localToWorld(load.localPoint);
        renderer.drawLine(from, from + load.force * ARROW_SCALE, Color(1.0f, 0.9f, 0.2f));
    }
}
