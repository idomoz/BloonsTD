//
// Created by Ido Mozes on 11/07/2019.
//

#include "ShotsSpawnSystem.h"

void ShotsSpawnSystem::update(Entities *layers, GameData &gameData) {
    for (auto &entity: layers[TOWERS_LAYER]) {
        auto[kind, towerRange, towerPosition, strategy] = entity->getComponents<Kind, Range, Position, Strategy>().value();
        float minDistance = MAP_HEIGHT + MAP_WIDTH;
        float minProgress = gameData.path.size();
        float maxProgress = -1;
        Entity *closestBloon = nullptr, *firstBloon = nullptr, *lastBloon = nullptr;
        for (auto &gameEntity: layers[BLOONS_LAYER]) {
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
            auto[velocityX, velocityY] = polarToCartesian(angle, 5);
            shot->addComponent<Velocity>(velocityX, velocityY);
            shot->addComponent<Type>(SHOT_T);
            shot->addComponent<Range>(5);
            SDL_Surface *surface = gameData.assets["Dart"];
            shot->addComponent<Visibility>(gameData.renderer, surface, SDL_Rect{0, 0, surface->w / 25},
                                           radToDeg(angle));
            layers[SHOTS_LAYER].emplace_back(shot);
        }
    }
}