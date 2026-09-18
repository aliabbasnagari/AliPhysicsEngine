#include "physics/PhysicsWorld.h"

PhysicsWorld::PhysicsWorld()
    : particle(Vec2(0.0f, 5.0f), Vec2(0.0f, 0.0f), 1.0f),
      explicitParticle(
          Vec2(3.0f, 0.0f),
          Vec2(0.0f, 0.0f),
          1.0f),
      semiImplicitParticle(
          Vec2(3.0f, 0.0f),
          Vec2(0.0f, 0.0f),
          1.0f),
      anchor(0.0f, 0.0f),
      restLength(2.0f),
      springConstant(10.0f)
{
}

void PhysicsWorld::step(float fixedDt)
{
    particle.integrate(fixedDt, Vec2(0.0f, -9.81f));

    auto springAcceleration = [this](const Particle &particle)
    {
        Vec2 displacement = particle.position - anchor;

        float length = displacement.length();

        if (length <= 0.0001f)
            return Vec2(0.0f, 0.0f);

        float extension = length - restLength;

        Vec2 direction = displacement / length;

        Vec2 force = direction * (-springConstant * extension);

        return force * particle.inverseMass;
    };

    Vec2 explicitAcceleration = springAcceleration(explicitParticle);
    Vec2 semiImplicitAcceleration = springAcceleration(semiImplicitParticle);

    explicitParticle.integrate(
        fixedDt,
        explicitAcceleration,
        IntegrationMode::ExplicitEuler);

    semiImplicitParticle.integrate(
        fixedDt,
        semiImplicitAcceleration,
        IntegrationMode::SemiImplicitEuler);
}

const Particle &PhysicsWorld::getParticle() const
{
    return particle;
}

const Particle &PhysicsWorld::getExplicitParticle() const
{
    return explicitParticle;
}

const Particle &PhysicsWorld::getSemiImplicitParticle() const
{
    return semiImplicitParticle;
}