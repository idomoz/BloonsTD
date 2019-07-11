//
// Created by Ido Mozes on 07/07/2019.
//

#include "SpawnSystem.h"


void SpawnSystem::update(Entities *layers, GameData &gameData) {
    for (int i = 0; i < N_LAYERS; ++i) {
        Entities newEntities;
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
                                                                     surface->w / 3, surface->h / 3});
                            bloon->addComponent<Range>(std::max(surface->w / 6, surface->h / 6));
                            newEntities.emplace_back(bloon);
                        }
                        break;
                    }
                    case TOWER_T: {
                        auto[kind, towerRange, towerPosition] = entity->getComponents<Kind, Range, Position>().value();
                        float minDistance = MAP_HEIGHT + MAP_WIDTH;
                        Point closestPosition = {-1, -1};
                        Entity * b;
                        for (auto &gameEntity: layers[GAME_LAYER]) {
                            float distance;
                            if (gameEntity->getComponent<Type>()->value == BLOON_T) {
                                auto[bloonRange, bloonPosition] = gameEntity->getComponents<Range, Position>().value();
                                distance =
                                        twoPointsDistance(bloonPosition.value, towerPosition.value) - bloonRange.value;
                                if (distance < minDistance) {
                                    minDistance = distance;
                                    closestPosition = {bloonPosition.value.X, bloonPosition.value.Y};
                                    b=gameEntity.get();
                                }
                            }
                        }
                        if (closestPosition.X != -1 and minDistance <= towerRange.value) {
                            auto shot = new Entity();
                            shot->addComponent<Position>(towerPosition.value.X, towerPosition.value.Y);
                            float angle = twoPointsAngle(towerPosition.value, closestPosition);
                            auto[velocityX, velocityY] = polarToCartesian(angle, 10);
                            shot->addComponent<Velocity>(velocityX, velocityY);
                            shot->addComponent<Type>(SHOT_T);
                            SDL_Surface * surface = gameData.assets["Dart"];
                            shot->addComponent<Visibility>(gameData.renderer, gameData.assets["Dart"],
                                                           SDL_Rect{0, 0, gameData.assets["Dart"]->w / 2});
                            newEntities.emplace_back(shot);
                        }

                        break;
                    }
                }
            }
        }
        if (!newEntities.empty())
            layers[i] += newEntities;
    }
}
