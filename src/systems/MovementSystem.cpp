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
                auto pathIndexP = entity->getComponent<PathIndex>();
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
                } else if (pathIndexP) {
                    if (auto regrowP = entity->getComponent<Regrow>()) {
                        auto[lives, kind] = entity->getComponents<Lives, Kind>().value();
                        regrowP->regrowTime -= 1;
                        if (regrowP->regrowTime == 0 and kind.value < regrowP->kind) {
                            switch (kind.value) {
                                case RED_BLOON:
                                case BLUE_BLOON:
                                case GREEN_BLOON:
                                case YELLOW_BLOON:
                                    kind.value += 1;
                                    break;
                                case PINK_BLOON: {
                                    switch (regrowP->kind) {
                                        case PURPLE_BLOON:
                                        case BLACK_BLOON:
                                        case WHITE_BLOON:
                                            kind.value = regrowP->kind;
                                            break;
                                        case LEAD_BLOON:
                                        case ZEBRA_BLOON:
                                        case RAINBOW_BLOON:
                                        case CERAMIC_BLOON:
                                            kind.value = BLACK_BLOON;
                                            break;
                                    }
                                    break;
                                }
                                case BLACK_BLOON:
                                    if (regrowP->kind == LEAD_BLOON)
                                        kind.value = LEAD_BLOON;
                                    else
                                        kind.value = ZEBRA_BLOON;
                                    break;
                                case WHITE_BLOON:
                                    kind.value = ZEBRA_BLOON;
                                case ZEBRA_BLOON:
                                case RAINBOW_BLOON:
                                    kind.value += 1;
                                    break;
                            }
                            lives.value = getBloonProperty<TOTAL_LIVES>(entity);

                            auto[texture,surface] = gameData.getTexture(getSurfaceName(entity));
                            visibility.setDstRect(SDL_Rect{0, 0, surface->w / 3, 0});
                            visibility.reloadTexture(texture, surface);
                            entity->getComponent<Range>()->value = std::max(surface->w / 6, surface->h / 6);
                            regrowP->regrowTime = 60;
                        }
                    }
                    auto &pathIndex = *pathIndexP;
                    float speed = getSpeed(entity);
                    if (auto gooP = entity->getComponent<Goo>()) {
                        if (gooP->kind == CORROSIVE and --gooP->timetoRecharge == 0){
                            entity->addComponent<DamageEvent>(gooP->damage, EntityP(nullptr));
                            gooP->timetoRecharge = gooP->interval;
                        }
                        gooP->ttl -= 1;
                        if (gooP->ttl == 0) {
                            entity->removeComponent<Goo>();
                            auto[texture,surface] = gameData.getTexture(getSurfaceName(entity));
                            auto &visibility = *entity->getComponent<Visibility>();
                            visibility.setDstRect(SDL_Rect{0, 0, surface->w / 3, 0});
                            visibility.reloadTexture(texture, surface);
                        }
                    }
                    pathIndex.progress += speed;
                    float deltaIndex = pathIndex.progress - pathIndex.index;
                    if (entity->getComponent<Kind>()->value >= MOAB) {
                        float angle = 0;
                        Point tempPosition = position.value;
                        for (int j = std::max(0, pathIndex.index - 10);
                             j < std::min(pathIndex.index + 10, int(gameData.path.size() - 1)); ++j) {
                            auto[tempDeltaX, tempDeltaY] = getDelta(gameData.path[j]);
                            tempPosition.X += tempDeltaX;
                            tempPosition.Y += tempDeltaY;
                        }
                        angle = radToDeg(twoPointsAngle(position.value, tempPosition));
                        visibility.angle = angle;
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
                    if (pathIndexP) {
                        gameData.lives -= entity->getComponent<Lives>()->value;
                        if (gameData.lives <= 0) {
                            gameData.lost = true;
                            gameData.lives = 0;
                        }

                    }
                    entity->addComponent<RemoveEntityEvent>();
                }
            }
        }
    }
}
