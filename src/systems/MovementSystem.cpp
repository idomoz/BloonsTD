//
// Created by Ido Mozes on 02/07/2019.
//

#include "MovementSystem.h"

enum Directions {
    RIGHT, BOTTOM_RIGHT, BOTTOM, BOTTOM_LEFT, LEFT, TOP_LEFT, TOP, TOP_RIGHT
};

std::tuple<int, int> getDelta(int direction) {
    switch (direction) {
        case RIGHT:
            return std::make_tuple(1, 0);
        case BOTTOM_RIGHT:
            return std::make_tuple(1, 1);
        case BOTTOM:
            return std::make_tuple(0, 1);
        case BOTTOM_LEFT:
            return std::make_tuple(-1, 1);
        case LEFT:
            return std::make_tuple(-1, 0);
        case TOP_LEFT:
            return std::make_tuple(-1, -1);
        case TOP:
            return std::make_tuple(0, -1);
        case TOP_RIGHT:
            return std::make_tuple(1, -1);
        default:
            return std::make_tuple(0, 0);
    }
}

void MovementSystem::update(Entities *layers, GameData &gameData) {
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
                        velocity.changeVelocity(acceleration.value.X, acceleration.value.Y);
                    }

                    deltaX = velocity.value.X;
                    deltaY = velocity.value.Y;
                    if (auto distanceP = entity->getComponent<Distance>()) {
                        auto &distance = distanceP->value;
                        auto[alpha, R] = cartesianToPolar(deltaX, deltaY);
                        if (distance == 0) {
                            entity->addComponent<RemoveEntityEvent>();
                        } else if (distance >= R) {
                            distance -= R;
                        } else {
                            std::tie(deltaX, deltaY) = polarToCartesian(alpha, distance);
                            distance = 0;
                        }
                    }
                } else if (auto pathIndexP = entity->getComponent<PathIndex>()) {
                    auto &pathIndex = *pathIndexP;
                    pathIndex.progress += getSpeed(entity);
                    float deltaIndex = pathIndex.progress - pathIndex.index;
                    if(entity->getComponent<Kind>()->value>=MOAB) {
                        float angle = 0;
                        Point tempPosition = position.value;
                        for (int j = std::max(0,pathIndex.index-10);
                             j < std::min(pathIndex.index + 10, int(gameData.path.size() - 1)); ++j) {
                            auto[tempDeltaX, tempDeltaY] = getDelta(gameData.path[j]);
                            tempPosition.X += tempDeltaX;
                            tempPosition.Y += tempDeltaY;
                        }
                        angle = radToDeg(twoPointsAngle( position.value,tempPosition));
                        visibility.angle=angle;
                    }
                    while (deltaIndex >= (gameData.path[pathIndex.index] % 2 == 0 ? 1 : sqrt(2))) {
                        auto[tempDeltaX, tempDeltaY] = getDelta(gameData.path[pathIndex.index]);
                        deltaX += tempDeltaX;
                        deltaY += tempDeltaY;
                        deltaIndex -= (gameData.path[pathIndex.index] % 2 == 0 ? 1 : sqrt(2));
                        if (pathIndex.index < gameData.path.size() - 1)
                            pathIndex.index++;
                    }
                }
                position.changePosition(deltaX, deltaY);
                if (position.value.X > MAP_WIDTH or position.value.X < 0 or position.value.Y > MAP_HEIGHT or
                    position.value.Y < 0) {
                    entity->addComponent<RemoveEntityEvent>();
                }
            }
        }
    }
}
