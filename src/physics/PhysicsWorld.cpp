#include "physics/PhysicsWorld.h"

#include "physics/GravityGenerator.h"
#include "physics/DragGenerator.h"

PhysicsWorld::PhysicsWorld()
    : semiImplicitParticle(
          Vec2(3.0f, 0.0f),
          Vec2(3.0f, 1.0f),
          1.0f),
      verletParticle(
          Vec2(3.0f, 0.0f), Vec2(3.0f, -1.0f), 1.0f, 0.016f),
      anchor(0.0f, 0.0f),
      restLength(2.0f),
      springConstant(10.0f)
{
    forceGenerators.push_back(
        std::make_unique<DragGenerator>(0.5f));

    forceGenerators.push_back(
        std::make_unique<GravityGenerator>(Vec2(0.0f, -9.81f)));
}

void PhysicsWorld::step(float fixedDt)
{
    semiImplicitParticle.clearForces();
    verletParticle.clearForces();

    for (const auto &generator : forceGenerators)
    {
        generator->updateForce(semiImplicitParticle, fixedDt);
        generator->updateForce(verletParticle, fixedDt);
    }

    semiImplicitParticle.integrate(
        fixedDt,
        IntegrationMode::SemiImplicitEuler);

    verletParticle.integrate(fixedDt);
}

const Particle &PhysicsWorld::getSemiImplicitParticle() const
{
    return semiImplicitParticle;
}

const VerletParticle &PhysicsWorld::getVerletParticle() const
{
    return verletParticle;
}