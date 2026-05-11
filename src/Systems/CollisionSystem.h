#ifndef COLLISIONSYSTEM_H
#define COLLISIONSYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/TransformComponent.h"

class CollisionSystem : public System {
public:
    CollisionSystem() {
        RequireComponent<TransformComponent>();
        RequireComponent<BoxColliderComponent>();
    }

    void Update() {
        auto entities = GetSystemEntities();

        for (auto i = entities.begin(); i != entities.end(); i++) {
            Entity a = *i;
            auto aTransform = a.GetComponent<TransformComponent>();
            auto aCollider = a.GetComponent<BoxColliderComponent>();
            for (auto j = i; j != entities.end(); ++j) {
                Entity b = *j;

                if (a == b) {
                    continue;
                }

                auto bTransform = b.GetComponent<TransformComponent>();
                auto bCollider = b.GetComponent<BoxColliderComponent>();

                bool hasIntersection = CheckAABBCollision(
                        glm::vec4(aTransform.position + aCollider.offset, aCollider.width, aCollider.height),
                        glm::vec4(bTransform.position + bCollider.offset, bCollider.width, bCollider.height)
                );

                if (hasIntersection) {
                    Logger::Log("Entity " + std::to_string(a.GetId()) + " is colliding with Entity " +
                                std::to_string(b.GetId()));
                }
            }
        }
    }

    bool CheckAABBCollision(glm::vec4 aCoordinates, glm::vec4 bCoordinates) {
        // width = z
        // height = w
        return (
                aCoordinates.x < bCoordinates.x + bCoordinates.z &&
                aCoordinates.x + aCoordinates.z > bCoordinates.x &&
                aCoordinates.y < bCoordinates.y + bCoordinates.w &&
                aCoordinates.y + aCoordinates.w > bCoordinates.y
        );
    }
};

#endif //COLLISIONSYSTEM_H
