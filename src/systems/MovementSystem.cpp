//
// Created by Ido Mozes on 02/07/2019.
//

#include "MovementSystem.h"


void MovementSystem::update(std::vector<std::shared_ptr<Entity>> &entities, GameData &gameData) {
    for (auto &entity: entities) {
        // Move all entities with Visibility and Position
        if (entity->hasComponents(createMask({ComponentType::VISIBILITY, ComponentType::POSITION}))) {
            auto &visibility = entity->getComponent<Visibility>();
            auto &position = entity->getComponent<Position>();
            float deltaX = 0, deltaY = 0;
            if (entity->hasComponents(createMask({ComponentType::VELOCITY}))) {
                auto &velocity = entity->getComponent<Velocity>();
                if (entity->hasComponents(createMask({ACCELERATION}))) {
                    auto &acceleration = entity->getComponent<Acceleration>();
                    velocity.changeVelocity(acceleration.getX(), acceleration.getY());
                }
                deltaX = velocity.getX();
                deltaY = velocity.getY();

            } else if (entity->hasComponents(createMask({ComponentType::PATH_INDEX, ComponentType::SPEED}))) {
                auto &pathIndex = entity->getComponent<PathIndex>();
                auto &speed = entity->getComponent<Speed>();
                pathIndex.progress += speed.speed;
                float deltaIndex = pathIndex.progress - pathIndex.index;
                while (deltaIndex >= (gameData.path[pathIndex.index] % 2 == 0 ? 1 : sqrt(2))) {
                    switch (gameData.path[pathIndex.index]) {
                        case 0:
                            deltaX += 1;
                            break;
                        case 1:
                            deltaX += 1;
                            deltaY += 1;
                            break;
                        case 2:
                            deltaY += 1;
                            break;
                        case 3:
                            deltaX += -1;
                            deltaY += 1;
                            break;
                        case 4:
                            deltaX += -1;
                            break;
                        case 5:
                            deltaX += -1;
                            deltaY += -1;
                            break;
                        case 6:
                            deltaY += -1;
                            break;
                        case 7:
                            deltaX += 1;
                            deltaY += -1;
                            break;
                    }
                    deltaIndex -= (gameData.path[pathIndex.index] % 2 == 0 ? 1 : sqrt(2));
                    if (pathIndex.index < gameData.path.size() - 1)
                        pathIndex.index++;
                }
            }
            position.changePosition(deltaX , deltaY);
        }
    }
}
