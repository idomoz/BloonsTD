//
// Created by Ido Mozes on 07/07/2019.
//

#include "SpawnSystem.h"


void SpawnSystem::update(Entities *layers, GameData &gameData) {
    Entities newEntities[N_LAYERS];
    for (int i = 0; i < N_LAYERS; ++i) {
        for (auto &entity: layers[i]) {
            if (auto typeP = entity->getComponent<Type>()) {
                switch (typeP->value) {
                    case SEQUENCE_T: {
                        auto[sequence, kind, speed] = entity->getComponents<Sequence, Kind, Speed>().value();
                        int amount = sequence.getAmountReady();
                        for (int j = 0; j < amount; ++j) {
                            auto *bloon = new Entity();
                            bloon->addComponent<Type>(BLOON_T);
                            bloon->addComponent<Kind>(kind);
                            bloon->addComponent<Position>(gameData.startingPoint.X, gameData.startingPoint.Y);
                            bloon->addComponent<PathIndex>(0);
                            bloon->addComponent<Speed>(speed.value);

                            SDL_Surface *surface = gameData.assets[kind.value];
                            bloon->addComponent<Visibility>(gameData.renderer, surface,
                                                            SDL_Rect{int(gameData.startingPoint.X),
                                                                     int(gameData.startingPoint.Y),
                                                                     int(surface->w / 3), int(surface->h / 3)});
                            bloon->addComponent<Range>(std::max(surface->w / 6, surface->h / 6));
                            newEntities[GAME_LAYER].emplace_back(bloon);
                        }
                        break;
                    }
                    case TOWER_T: {
                        auto[kind, towerRange, towerPosition, strategy] = entity->getComponents<Kind, Range, Position, Strategy>().value();
                        float minDistance = MAP_HEIGHT + MAP_WIDTH;
                        float minProgress = gameData.path.size();
                        float maxProgress = -1;
                        Entity *closestBloon = nullptr, *firstBloon = nullptr, *lastBloon = nullptr;
                        for (auto &gameEntity: layers[GAME_LAYER]) {
                            float distance;
                            if (gameEntity->getComponent<Type>()->value == BLOON_T) {
                                auto[bloonRange, bloonPosition, pathIndex] = gameEntity->getComponents<Range, Position, PathIndex>().value();
                                distance =
                                        twoPointsDistance(bloonPosition.value, towerPosition.value) - bloonRange.value;
                                if (distance > towerRange.value)
                                    continue;
                                if (distance < minDistance) {
                                    minDistance = distance;
                                    closestBloon = gameEntity.get();
                                }
                                if (pathIndex.progress < minProgress) {
                                    minProgress = pathIndex.progress;
                                    lastBloon = gameEntity.get();
                                }
                                if (pathIndex.progress > maxProgress) {
                                    maxProgress = pathIndex.progress;
                                    firstBloon = gameEntity.get();
                                }

                            }
                        }
                        if (closestBloon or firstBloon or lastBloon) {
                            Entity *target;
                            switch (strategy.value) {
                                case CLOSEST:
                                    target = closestBloon;
                                    break;
                                case FIRST:
                                    target = firstBloon;
                                    break;
                                case LAST:
                                    target = lastBloon;
                                    break;
                            }
                            auto shot = new Entity();
                            shot->addComponent<Position>(towerPosition.value.X, towerPosition.value.Y);
                            float angle = twoPointsAngle(towerPosition.value, target->getComponent<Position>()->value);
                            auto[velocityX, velocityY] = polarToCartesian(angle, 10);
                            shot->addComponent<Velocity>(velocityX, velocityY);
                            shot->addComponent<Type>(SHOT_T);
                            SDL_Surface *surface = gameData.assets["Dart"];
                            shot->addComponent<Visibility>(gameData.renderer, surface, SDL_Rect{0, 0, surface->w / 25},
                                                           radToDeg(angle));
                            newEntities[SHOTS_LAYER].emplace_back(shot);
                        }

                        break;
                    }
                }
            }
        }
    }
    for (int i = 0; i < N_LAYERS; ++i) {
        if (!newEntities[i].empty())
            layers[i] += newEntities[i];
    }
}
