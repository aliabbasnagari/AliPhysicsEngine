#include "physics/BroadPhase.h"
#include "physics/AABB.h"

std::vector<BodyPair> findPairsBruteForce(const std::vector<std::unique_ptr<RigidBody>> &bodies)
{
    // Compute each AABB once, not once per pair.
    std::vector<AABB> boxes;
    boxes.reserve(bodies.size());
    for (const auto &body : bodies)
    {
        boxes.push_back(body->computeAABB());
    }

    std::vector<BodyPair> pairs;
    for (size_t i = 0; i < bodies.size(); ++i)
    {
        for (size_t j = i + 1; j < bodies.size(); ++j)
        {
            if (bodies[i]->isStatic() && bodies[j]->isStatic())
            {
                continue;
            }

            if (boxes[i].overlaps(boxes[j]))
            {
                pairs.push_back({bodies[i].get(), bodies[j].get()});
            }
        }
    }
    return pairs;
}