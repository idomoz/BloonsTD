//
// Created by Ido Mozes on 11/07/2019.
//

#include "ShotsSpawnSystem.h"

void ShotsSpawnSystem::update(Entities *layers, GameData &gameData) {
    if (!gameData.levelRunning)
        return;
    for (auto &entity: layers[TOWERS_LAYER]) {
        auto[kind, shotKind, towerRange, towerPosition, strategy, attackSpeed, pierce, damage, distance, visibility] =
        entity->getComponents<Kind, ShotKind, Range, Position, Strategy, AttackSpeed, Pierce, Damage, Distance, Visibility>().value();
        auto camoP = entity->getComponent<Camo>();
        float minDistance = MAP_HEIGHT + MAP_WIDTH;
        float minProgress = gameData.path.size();
        float maxProgress = -1;
        float maxLives = -1;
        Entity *closestBloon = nullptr, *firstBloon = nullptr, *lastBloon = nullptr, *strongestBloon = nullptr;
        for (auto &gameEntity: layers[BLOONS_LAYER]) {
            if (gameEntity->getComponent<Camo>() and !camoP)
                continue;
            float distance;
            if (gameEntity->getComponent<Type>()->value == BLOON_T) {
                auto[bloonRange, bloonPosition, pathIndex, lives] = gameEntity->getComponents<Range, Position, PathIndex, Lives>().value();
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
                if (lives.value > maxLives or (lives.value == maxLives and pathIndex.progress > maxProgress)) {
                    maxLives = lives.value;
                    strongestBloon = gameEntity.get();
                }
                if (pathIndex.progress > maxProgress) {
                    maxProgress = pathIndex.progress;
                    firstBloon = gameEntity.get();
                }

            }
        }
        if (closestBloon or firstBloon or lastBloon or strongestBloon) {
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
                case STRONGEST:
                    target = strongestBloon;
                    break;
            }

            int amount = attackSpeed.getAmountReady();
            float angle = twoPointsAngle(towerPosition.value, target->getComponent<Position>()->value);
            for (int i = 0; i < amount; ++i) {
                switch (shotKind.value) {
                    case BOMB:
                    case ENHANCED_BOMB:
                    case MISSILE:
                    case MOAB_MAULER:
                    case MOAB_ASSASSIN:
                    case MOAB_ELIMINATOR:
                    case GOO_SHOT:
                    case LASER:
                    case PLASMA:
                    case SUN:
                    case SPIKE:
                    case JUGGERNAUT:
                    case DART: {
                        int shotsAmount = 1;
                        auto shotsAmountP = entity->getComponent<ShotsAmount>();
                        float shotAngle = angle;
                        float deltaAngle = 10;
                        if (shotsAmountP) {
                            shotsAmount = shotsAmountP->value;
                            shotAngle -= degToRad((deltaAngle * (shotsAmount - 1)) / 2.0);
                            deltaAngle = degToRad(deltaAngle);
                        }

                        for (int j = 0; j < shotsAmount; ++j) {
                            EntityP shot(new Entity());
                            shot->addComponent<Position>(towerPosition.value.X, towerPosition.value.Y);
                            shot->addComponent<Type>(SHOT_T);
                            shot->addComponent<Kind>(shotKind.value);
                            auto[velocityX, velocityY] = polarToCartesian(shotAngle, getSpeed(shot));
                            shot->addComponent<Velocity>(velocityX, velocityY);
                            shot->addComponent<Range>(5);
                            if (auto spreadP = entity->getComponent<Spread>())
                                shot->addComponent<Spread>(*spreadP);
                            if (auto gooP = entity->getComponent<Goo>())
                                shot->addComponent<Goo>(*gooP);

                            if (entity->getComponent<MoabClassAffecting>())
                                shot->addComponent<MoabClassAffecting>();
                            shot->addComponents(pierce, damage, distance);
                            if (camoP)
                                shot->addComponent<Camo>();
                            shot->addComponent<PoppedBloons>();
                            auto[texture, surface] = gameData.getTexture(getSurfaceName(shot));
                            float scale = 1;
                            switch (shotKind.value) {
                                case MISSILE:
                                    scale = 1.5;
                                    break;
                                case DART:
                                case MOAB_MAULER:
                                case MOAB_ASSASSIN:
                                case MOAB_ELIMINATOR:
                                    scale = 2;
                                    break;
                                case BOMB:
                                case ENHANCED_BOMB:
                                case SPIKE:
                                case JUGGERNAUT:
                                    scale = 4;
                                    break;
                                case LASER:
                                    scale = 10;
                                    break;
                                case PLASMA:
                                case SUN:
                                    scale = 6;
                                    break;
                            }
                            shot->addComponent<Visibility>(texture, surface,
                                                           SDL_Rect{0, 0, int(surface->w / scale)},
                                                           radToDeg(shotAngle));
                            layers[SHOTS_LAYER].emplace_back(shot);
                            shotAngle += deltaAngle;
                        }
                        break;
                    }

                    case BULLET:
                    case ENHANCED_BULLET: {
                        if (shotKind.value == BULLET and target->getComponent<Kind>()->value == LEAD_BLOON)
                            break;
                        EntityP shot(new Entity());
                        shot->addComponent<PoppedBloons>();
                        shot->addComponent<Kind>(shotKind.value);
                        target->addComponent<DamageEvent>(
                                target->getComponent<Camo>() ? damage.value * 2 : damage.value, shot);
                        break;
                    }
                    case TACK:
                    case HOT_TACK:
                    case BLADE:
                    case ENHANCED_TACK: {
                        int shotsAmount = entity->getComponent<ShotsAmount>()->value;
                        for (int j = 0; j < shotsAmount; ++j) {
                            EntityP shot(new Entity());
                            shot->addComponent<Type>(SHOT_T);
                            shot->addComponent<Kind>(shotKind.value);
                            angle = (2 * M_PI / shotsAmount) * j;
                            auto[velocityX, velocityY] = polarToCartesian(angle, getSpeed(shot));
                            shot->addComponent<Velocity>(velocityX, velocityY);
                            auto[deltaX, deltaY] = polarToCartesian(angle, 10);
                            shot->addComponent<Position>(towerPosition.value.X + deltaX,
                                                         towerPosition.value.Y + deltaY);
                            shot->addComponent<Range>(5);
                            shot->addComponents(pierce, damage, distance);
                            shot->addComponent<PoppedBloons>();
                            auto[texture, surface] = gameData.getTexture(getSurfaceName(shot));
                            int scale = 1;
                            switch (shotKind.value) {
                                case TACK:
                                case HOT_TACK:
                                case ENHANCED_TACK:
                                    scale = 5;
                                    break;
                                case BLADE:
                                    scale = 4;
                                    break;
                            }
                            shot->addComponent<Visibility>(texture, surface,
                                                           SDL_Rect{0, 0, surface->w / scale},
                                                           radToDeg(angle));
                            layers[SHOTS_LAYER].emplace_back(shot);
                        }
                        break;
                    }
                }
            }
            if (shotKind.value != TACK and shotKind.value != ENHANCED_TACK and shotKind.value != BLADE and
                shotKind.value != HOT_TACK)
                visibility.angle = radToDeg(angle) + 90;
        } else
            attackSpeed.recharge();
    }

}