//
// Created by Ido Mozes on 11/07/2019.
//

#include "ShotsSpawnSystem.h"

void ShotsSpawnSystem::update(Entities *layers, GameData &gameData) {
    for (auto &entity: layers[TOWERS_LAYER]) {
        auto[kind, shotKind, towerRange, towerPosition, strategy, attackSpeed, pierce, damage, distance, visibility] =
        entity->getComponents<Kind, ShotKind, Range, Position, Strategy, AttackSpeed, Pierce, Damage, Distance, Visibility>().value();
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
            int amount = attackSpeed.getAmountReady();
            float angle = twoPointsAngle(towerPosition.value, target->getComponent<Position>()->value);
            for (int i = 0; i < amount; ++i) {
                switch (shotKind.value){
                    case DART:{
                        EntityP shot(new Entity());
                        shot->addComponent<Position>(towerPosition.value.X, towerPosition.value.Y);
                        shot->addComponent<Type>(SHOT_T);
                        shot->addComponent<Kind>(shotKind.value);
                        auto[velocityX, velocityY] = polarToCartesian(angle, getSpeed(shot));
                        shot->addComponent<Velocity>(velocityX, velocityY);
                        shot->addComponent<Range>(5);
                        shot->addComponents(pierce, damage, distance);
                        shot->addComponent<PoppedBloons>();
                        SDL_Surface *surface = gameData.assets[getSurfaceName(shot)];
                        shot->addComponent<Visibility>(gameData.renderer, surface, SDL_Rect{0, 0, surface->w / 25},
                                                       radToDeg(angle));
                        layers[SHOTS_LAYER].emplace_back(shot);
                        break;
                    }
                    case GUN:
                    {
                        target->addComponent<DamageEvent>(damage.value,EntityP(nullptr));
                    }

                }

            }
            visibility.angle = radToDeg(angle) + 90;
        } else
            attackSpeed.recharge();
    }
}