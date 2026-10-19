#pragma once

#include <memory>
#include <vector>

#include "physics/RigidBody.h"

struct BodyPair
{
    const RigidBody *a;
    const RigidBody *b;
};

std::vector<BodyPair> findPairsBruteForce(
    const std::vector<std::unique_ptr<RigidBody>> &bodies);