//
// Created by Ido Mozes on 02/07/2019.
//

#include "MovementSystem.h"


void MovementSystem::update(std::vector<std::shared_ptr<Entity>> *layers, GameData &gameData) {
    for (int i = 0; i < N_LAYERS; ++i) {
        for (auto &entity: layers[i]) {
            // Move all entities with Visibility and Position
            if (auto components = entity->getComponents<Visibility, Position>()) {
                auto[visibility, position]=components.value();
                float deltaX = 0, deltaY = 0;
                if (auto velocityP = entity->getComponent<Velocity>()) {
                    auto &velocity = *velocityP;
                    if (auto accelerationP = entity->getComponent<Acceleration>()) {
                        auto &acceleration = *accelerationP;
                        velocity.changeVelocity(acceleration.getX(), acceleration.getY());
                    }
                    deltaX = velocity.getX();
                    deltaY = velocity.getY();

                } else if (auto components2 = entity->getComponents<PathIndex, Speed>()) {
                    auto[pathIndex, speed] =components2.value();
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
                position.changePosition(deltaX, deltaY);
            }
        }
    }
}
