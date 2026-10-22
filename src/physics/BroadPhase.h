#pragma once

#include <memory>
#include <vector>

#include "physics/RigidBody.h"
#include "graphics/Renderer.h"

struct BodyPair
{
    const RigidBody *a;
    const RigidBody *b;
};

std::vector<BodyPair> findPairsBruteForce(
    const std::vector<std::unique_ptr<RigidBody>> &bodies);

std::vector<BodyPair> findPairsSpatialGrid(
    const std::vector<std::unique_ptr<RigidBody>> &bodies,
    float cellSize);

void drawSpatialGrid(
    Renderer &renderer,
    const std::vector<std::unique_ptr<RigidBody>> &bodies,
    float cellSize,
    Color color);