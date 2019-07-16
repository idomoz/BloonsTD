//
// Created by Ido Mozes on 15/07/2019.
//

#include "DamageSystem.h"

bool didBloonPop(EntityP &bloon, int &lives, int &damage) {
    if (bloon->getComponent<Fortified>()) {
        if (damage == 1) {
            lives -= damage;
            if (lives % 2 == 1)
                return false;
        } else {
            lives -= 2;
            damage -= 2;
        }
    } else {
        lives -= 1;
        damage -= 1;
    }
    return true;
}

bool didBloonPop(EntityP &bloon, int &lives, int &damage, int minLives) {
    if (lives - damage >= minLives) {
        lives -= damage;
        return false;
    }
    damage -= lives - minLives + 1;
    lives = minLives - 1;
    return true;
}

void damageBloon(EntityP &bloon, EntityP &shot, int damage, GameData &gameData, Entities &newBloons) {
    if (damage == 0)
        return;
    auto &lives = bloon->getComponent<Lives>()->value;
    if (damage >= lives) {
        bloon->addComponent<RemoveEntityEvent>();
        return;
    }
    auto &kind = bloon->getComponent<Kind>()->value;
    auto &visibility = *bloon->getComponent<Visibility>();
    auto[regrowP, camoP, fortifiedP, glueP, gumP, corrosiveP] = bloon->getComponentsP<Regrow, Camo, Fortified, Glue, Gum, Corrosive>();
    SDL_Surface *surface;
    switch (kind) {
        case RED_BLOON:
            lives -= damage;
            break;
        case BLUE_BLOON:
        case GREEN_BLOON:
        case YELLOW_BLOON:
        case PINK_BLOON: {
            lives -= damage;
            if (fortifiedP and damage == 1 and lives % 2 == 1)
                break;

            kind = (fortifiedP ? lives / 2 : lives) - 1;
            surface = gameData.assets[getSurfaceName(bloon)];
            visibility.setDstRect(SDL_Rect{0, 0, surface->w / 3, 0});
            visibility.loadTexture(gameData.renderer, surface);
            bloon->getComponent<Range>()->value = std::max(surface->w / 6, surface->h / 6);
            break;
        }
        case PURPLE_BLOON:
        case WHITE_BLOON:
        case BLACK_BLOON: {
            if (!didBloonPop(bloon, lives, damage))
                break;
            auto &pathIndex = *bloon->getComponent<PathIndex>();
            for (int i = 0; i < 2; ++i) {
                EntityP newBloon(new Entity());
                shot->getComponent<PoppedBloons>()->value.emplace(newBloon.get());
                newBloon->addComponent<Type>(BLOON_T);
                newBloon->addComponent<Kind>(PINK_BLOON);
                newBloon->addComponent<Lives>(lives / 2);
                newBloon->addComponent<Position>(gameData.startingPoint);
                newBloon->addComponent<PathIndex>(0);
                newBloon->getComponent<PathIndex>()->progress =
                        i == 0 ? std::fmaxf(0, pathIndex.progress - 10) : std::fminf(gameData.path.size() - 1,
                                                                                     pathIndex.progress + 10);
                if (regrowP)
                    newBloon->addComponent<Regrow>();
                if (camoP)
                    newBloon->addComponent<Camo>();
                if (gumP)
                    newBloon->addComponent<Gum>();
                else if (glueP)
                    newBloon->addComponent<Glue>();
                else if (corrosiveP)
                    newBloon->addComponent<Corrosive>();
                surface = gameData.assets[getSurfaceName(newBloon)];
                newBloon->addComponent<Range>(std::max(surface->w / 6, surface->h / 6));
                newBloon->addComponent<Visibility>(gameData.renderer, surface, SDL_Rect{0, 0, surface->w / 3, 0});
                newBloons.emplace_back(newBloon);
                damageBloon(newBloon, shot, i == 0 ? ceilf(damage / 2.0) : floorf(damage / 2.0), gameData, newBloons);
            }
            bloon->addComponent<RemoveEntityEvent>();
            break;
        }
        case ZEBRA_BLOON: {
            if (!didBloonPop(bloon, lives, damage))
                break;
            auto &pathIndex = *bloon->getComponent<PathIndex>();
            for (int i = 0; i < 2; ++i) {
                EntityP newBloon(new Entity());
                shot->getComponent<PoppedBloons>()->value.emplace(newBloon.get());
                newBloon->addComponent<Type>(BLOON_T);
                newBloon->addComponent<Kind>(i == 0 ? BLACK_BLOON : WHITE_BLOON);
                newBloon->addComponent<Lives>(lives / 2);
                newBloon->addComponent<Position>(gameData.startingPoint);
                newBloon->addComponent<PathIndex>(0);
                newBloon->getComponent<PathIndex>()->progress =
                        i == 0 ? std::fmaxf(0, pathIndex.progress - 10) : std::fminf(gameData.path.size() - 1,
                                                                                     pathIndex.progress + 10);
                if (regrowP)
                    newBloon->addComponent<Regrow>();
                if (camoP)
                    newBloon->addComponent<Camo>();
                if (gumP)
                    newBloon->addComponent<Gum>();
                else if (glueP)
                    newBloon->addComponent<Glue>();
                else if (corrosiveP)
                    newBloon->addComponent<Corrosive>();
                surface = gameData.assets[getSurfaceName(newBloon)];
                newBloon->addComponent<Range>(std::max(surface->w / 6, surface->h / 6));
                newBloon->addComponent<Visibility>(gameData.renderer, surface, SDL_Rect{0, 0, surface->w / 3, 0});
                newBloons.emplace_back(newBloon);
                damageBloon(newBloon, shot, i == 0 ? ceilf(damage / 2.0) : floorf(damage / 2.0), gameData, newBloons);
            }
            bloon->addComponent<RemoveEntityEvent>();
            break;
        }
        case LEAD_BLOON: {
            if (!didBloonPop(bloon, lives, damage))
                break;
            auto &pathIndex = *bloon->getComponent<PathIndex>();
            for (int i = 0; i < 2; ++i) {
                EntityP newBloon(new Entity());
                shot->getComponent<PoppedBloons>()->value.emplace(newBloon.get());
                newBloon->addComponent<Type>(BLOON_T);
                newBloon->addComponent<Kind>(BLACK_BLOON);
                newBloon->addComponent<Lives>(lives / 2);
                newBloon->addComponent<Position>(gameData.startingPoint);
                newBloon->addComponent<PathIndex>(0);
                newBloon->getComponent<PathIndex>()->progress =
                        i == 0 ? std::fmaxf(0, pathIndex.progress - 10) : std::fminf(gameData.path.size() - 1,
                                                                                     pathIndex.progress + 10);
                if (regrowP)
                    newBloon->addComponent<Regrow>();
                if (camoP)
                    newBloon->addComponent<Camo>();
                if (gumP)
                    newBloon->addComponent<Gum>();
                else if (glueP)
                    newBloon->addComponent<Glue>();
                else if (corrosiveP)
                    newBloon->addComponent<Corrosive>();
                surface = gameData.assets[getSurfaceName(newBloon)];
                newBloon->addComponent<Range>(std::max(surface->w / 6, surface->h / 6));
                newBloon->addComponent<Visibility>(gameData.renderer, surface, SDL_Rect{0, 0, surface->w / 3, 0});
                newBloons.emplace_back(newBloon);
                damageBloon(newBloon, shot, i == 0 ? ceilf(damage / 2.0) : floorf(damage / 2.0), gameData, newBloons);
            }
            bloon->addComponent<RemoveEntityEvent>();
            break;
        }
        case RAINBOW_BLOON: {
            if (!didBloonPop(bloon, lives, damage))
                break;
            auto &pathIndex = *bloon->getComponent<PathIndex>();
            for (int i = 0; i < 2; ++i) {
                EntityP newBloon(new Entity());
                shot->getComponent<PoppedBloons>()->value.emplace(newBloon.get());
                newBloon->addComponent<Type>(BLOON_T);
                newBloon->addComponent<Kind>(ZEBRA_BLOON);
                newBloon->addComponent<Lives>(lives / 2);
                newBloon->addComponent<Position>(gameData.startingPoint);
                newBloon->addComponent<PathIndex>(0);
                newBloon->getComponent<PathIndex>()->progress =
                        i == 0 ? std::fmaxf(0, pathIndex.progress - 10) : std::fminf(gameData.path.size() - 1,
                                                                                     pathIndex.progress + 10);
                if (regrowP)
                    newBloon->addComponent<Regrow>();
                if (camoP)
                    newBloon->addComponent<Camo>();
                if (gumP)
                    newBloon->addComponent<Gum>();
                else if (glueP)
                    newBloon->addComponent<Glue>();
                else if (corrosiveP)
                    newBloon->addComponent<Corrosive>();
                surface = gameData.assets[getSurfaceName(newBloon)];
                newBloon->addComponent<Range>(std::max(surface->w / 6, surface->h / 6));
                newBloon->addComponent<Visibility>(gameData.renderer, surface, SDL_Rect{0, 0, surface->w / 3, 0});
                newBloons.emplace_back(newBloon);
                damageBloon(newBloon, shot, i == 0 ? ceilf(damage / 2.0) : floorf(damage / 2.0), gameData, newBloons);
            }
            bloon->addComponent<RemoveEntityEvent>();
            break;
        }
        case CERAMIC_BLOON: {
            if (didBloonPop(bloon, lives, damage, getLives<MIN_LIVES>(bloon))) {
                auto &pathIndex = *bloon->getComponent<PathIndex>();
                for (int i = 0; i < 2; ++i) {
                    EntityP newBloon(new Entity());
                    shot->getComponent<PoppedBloons>()->value.emplace(newBloon.get());
                    newBloon->addComponent<Type>(BLOON_T);
                    newBloon->addComponent<Kind>(RAINBOW_BLOON);
                    newBloon->addComponent<Lives>(lives / 2);
                    newBloon->addComponent<Position>(gameData.startingPoint);
                    newBloon->addComponent<PathIndex>(0);
                    newBloon->getComponent<PathIndex>()->progress =
                            i == 0 ? std::fmaxf(0, pathIndex.progress - 10) : std::fminf(gameData.path.size() - 1,
                                                                                         pathIndex.progress + 10);
                    if (regrowP)
                        newBloon->addComponent<Regrow>();
                    if (camoP)
                        newBloon->addComponent<Camo>();
                    if (gumP)
                        newBloon->addComponent<Gum>();
                    else if (glueP)
                        newBloon->addComponent<Glue>();
                    else if (corrosiveP)
                        newBloon->addComponent<Corrosive>();
                    surface = gameData.assets[getSurfaceName(newBloon)];
                    newBloon->addComponent<Range>(std::max(surface->w / 6, surface->h / 6));
                    newBloon->addComponent<Visibility>(gameData.renderer, surface, SDL_Rect{0, 0, surface->w / 3, 0});
                    newBloons.emplace_back(newBloon);
                    damageBloon(newBloon, shot, i == 0 ? ceilf(damage / 2.0) : floorf(damage / 2.0), gameData,
                                newBloons);
                }
                bloon->addComponent<RemoveEntityEvent>();
                break;
            } else {
                surface = gameData.assets[getSurfaceName(bloon)];
                visibility.loadTexture(gameData.renderer, surface);
            }
            break;
        }
        case MOAB: {
            if (didBloonPop(bloon, lives, damage, getLives<MIN_LIVES>(bloon))) {
                auto &pathIndex = *bloon->getComponent<PathIndex>();
                for (int i = 0; i < 4; ++i) {
                    EntityP newBloon(new Entity());
                    shot->getComponent<PoppedBloons>()->value.emplace(newBloon.get());
                    newBloon->addComponent<Type>(BLOON_T);
                    newBloon->addComponent<Kind>(CERAMIC_BLOON);
                    newBloon->addComponent<Lives>(lives / 4);
                    newBloon->addComponent<Position>(gameData.startingPoint);
                    newBloon->addComponent<PathIndex>(0);
                    newBloon->getComponent<PathIndex>()->progress =
                            i < 2 ? std::fmaxf(0, pathIndex.progress - 20 + 10 * (i % 2)) : std::fminf(
                                    gameData.path.size() - 1, pathIndex.progress + 20 - 10 * (i % 2));
                    if (fortifiedP)
                        newBloon->addComponent<Fortified>();
                    surface = gameData.assets[getSurfaceName(newBloon)];
                    newBloon->addComponent<Range>(std::max(surface->w / 6, surface->h / 6));
                    newBloon->addComponent<Visibility>(gameData.renderer, surface, SDL_Rect{0, 0, surface->w / 3, 0});
                    newBloons.emplace_back(newBloon);
                    damageBloon(newBloon, shot, i == 0 ? ceilf(damage / 4.0) : damage - ceilf(damage / 4.0) * 3,
                                gameData, newBloons);
                }
                bloon->addComponent<RemoveEntityEvent>();
                break;
            } else {
                surface = gameData.assets[getSurfaceName(bloon)];
                visibility.loadTexture(gameData.renderer, surface);
            }
            break;
        }
        case BFB:{
            if (didBloonPop(bloon, lives, damage, getLives<MIN_LIVES>(bloon))) {
                auto &pathIndex = *bloon->getComponent<PathIndex>();
                for (int i = 0; i < 4; ++i) {
                    EntityP newBloon(new Entity());
                    shot->getComponent<PoppedBloons>()->value.emplace(newBloon.get());
                    newBloon->addComponent<Type>(BLOON_T);
                    newBloon->addComponent<Kind>(MOAB);
                    newBloon->addComponent<Lives>(lives / 4);
                    newBloon->addComponent<Position>(gameData.startingPoint);
                    newBloon->addComponent<PathIndex>(0);
                    newBloon->getComponent<PathIndex>()->progress =
                            i < 2 ? std::fmaxf(0, pathIndex.progress - 20 + 10 * (i % 2)) : std::fminf(
                                    gameData.path.size() - 1, pathIndex.progress + 20 - 10 * (i % 2));
                    if (fortifiedP)
                        newBloon->addComponent<Fortified>();
                    surface = gameData.assets[getSurfaceName(newBloon)];
                    newBloon->addComponent<Range>(std::max(surface->w / 6, surface->h / 6));
                    newBloon->addComponent<Visibility>(gameData.renderer, surface, SDL_Rect{0, 0, surface->w / 3, 0});
                    newBloons.emplace_back(newBloon);
                    damageBloon(newBloon, shot, i == 0 ? ceilf(damage / 4.0) : damage - ceilf(damage / 4.0) * 3,
                                gameData, newBloons);
                }
                bloon->addComponent<RemoveEntityEvent>();
                break;
            } else {
                surface = gameData.assets[getSurfaceName(bloon)];
                visibility.loadTexture(gameData.renderer, surface);
            }
            break;
        }
        case DDT:{
            if (didBloonPop(bloon, lives, damage, getLives<MIN_LIVES>(bloon))) {
                auto &pathIndex = *bloon->getComponent<PathIndex>();
                for (int i = 0; i < 4; ++i) {
                    EntityP newBloon(new Entity());
                    shot->getComponent<PoppedBloons>()->value.emplace(newBloon.get());
                    newBloon->addComponent<Type>(BLOON_T);
                    newBloon->addComponent<Kind>(CERAMIC_BLOON);
                    newBloon->addComponent<Lives>(lives / 4);
                    newBloon->addComponent<Position>(gameData.startingPoint);
                    newBloon->addComponent<PathIndex>(0);
                    newBloon->getComponent<PathIndex>()->progress =
                            i < 2 ? std::fmaxf(0, pathIndex.progress - 20 + 10 * (i % 2)) : std::fminf(
                                    gameData.path.size() - 1, pathIndex.progress + 20 - 10 * (i % 2));
                    if (fortifiedP)
                        newBloon->addComponent<Fortified>();
                    newBloon->addComponent<Regrow>();
                    newBloon->addComponent<Camo>();
                    surface = gameData.assets[getSurfaceName(newBloon)];
                    newBloon->addComponent<Range>(std::max(surface->w / 6, surface->h / 6));
                    newBloon->addComponent<Visibility>(gameData.renderer, surface, SDL_Rect{0, 0, surface->w / 3, 0});
                    newBloons.emplace_back(newBloon);
                    damageBloon(newBloon, shot, i == 0 ? ceilf(damage / 4.0) : damage - ceilf(damage / 4.0) * 3,
                                gameData, newBloons);
                }
                bloon->addComponent<RemoveEntityEvent>();
                break;
            } else {
                surface = gameData.assets[getSurfaceName(bloon)];
                visibility.loadTexture(gameData.renderer, surface);
            }
            break;
        }
        case ZOMG:{
            if (didBloonPop(bloon, lives, damage, getLives<MIN_LIVES>(bloon))) {
                auto &pathIndex = *bloon->getComponent<PathIndex>();
                for (int i = 0; i < 4; ++i) {
                    EntityP newBloon(new Entity());
                    shot->getComponent<PoppedBloons>()->value.emplace(newBloon.get());
                    newBloon->addComponent<Type>(BLOON_T);
                    newBloon->addComponent<Kind>(BFB);
                    newBloon->addComponent<Lives>(lives / 4);
                    newBloon->addComponent<Position>(gameData.startingPoint);
                    newBloon->addComponent<PathIndex>(0);
                    newBloon->getComponent<PathIndex>()->progress =
                            i < 2 ? std::fmaxf(0, pathIndex.progress - 20 + 10 * (i % 2)) : std::fminf(
                                    gameData.path.size() - 1, pathIndex.progress + 20 - 10 * (i % 2));
                    if (fortifiedP)
                        newBloon->addComponent<Fortified>();
                    surface = gameData.assets[getSurfaceName(newBloon)];
                    newBloon->addComponent<Range>(std::max(surface->w / 6, surface->h / 6));
                    newBloon->addComponent<Visibility>(gameData.renderer, surface, SDL_Rect{0, 0, surface->w / 3, 0});
                    newBloons.emplace_back(newBloon);
                    damageBloon(newBloon, shot, i == 0 ? ceilf(damage / 4.0) : damage - ceilf(damage / 4.0) * 3,
                                gameData, newBloons);
                }
                bloon->addComponent<RemoveEntityEvent>();
                break;
            } else {
                surface = gameData.assets[getSurfaceName(bloon)];
                visibility.loadTexture(gameData.renderer, surface);
            }
            break;
        }
        case BAD:{
            if (didBloonPop(bloon, lives, damage, getLives<MIN_LIVES>(bloon))) {
                auto &pathIndex = *bloon->getComponent<PathIndex>();
                for (int i = 0; i < 5; ++i) {
                    EntityP newBloon(new Entity());
                    shot->getComponent<PoppedBloons>()->value.emplace(newBloon.get());
                    newBloon->addComponent<Type>(BLOON_T);
                    newBloon->addComponent<Kind>(i<2?ZOMG:DDT);
                    if (fortifiedP)
                        newBloon->addComponent<Fortified>();
                    if(i>=2)
                        newBloon->addComponent<Camo>();
                    newBloon->addComponent<Lives>(getLives<TOTAL_LIVES>(newBloon));
                    newBloon->addComponent<Position>(gameData.startingPoint);
                    newBloon->addComponent<PathIndex>(0);
                    newBloon->getComponent<PathIndex>()->progress =
                            i < 2 ? std::fmaxf(0, pathIndex.progress - 20 + 10 * (i % 2)) : std::fminf(
                                    gameData.path.size() - 1, pathIndex.progress + 30 - 10 * (i % 3));
                    surface = gameData.assets[getSurfaceName(newBloon)];
                    newBloon->addComponent<Range>(std::max(surface->w / 6, surface->h / 6));
                    newBloon->addComponent<Visibility>(gameData.renderer, surface, SDL_Rect{0, 0, surface->w / 3, 0});
                    newBloons.emplace_back(newBloon);
                    damageBloon(newBloon, shot, i == 0 ? ceilf(damage / 5.0) : damage - ceilf(damage / 5.0) * 4,
                                gameData, newBloons);
                }
                bloon->addComponent<RemoveEntityEvent>();
                break;
            } else {
                surface = gameData.assets[getSurfaceName(bloon)];
                visibility.loadTexture(gameData.renderer, surface);
            }
            break;
        }
    }
}

void DamageSystem::update(Entities *layers, GameData &gameData) {
    Entities newBloons;
    for (auto &bloon: layers[BLOONS_LAYER]) {
        if (auto damageEventP = bloon->getComponent<DamageEvent>()) {
            auto &damageEvent = *damageEventP;
            auto &lives = bloon->getComponent<Lives>()->value;
            if (lives < damageEvent.damage)
                bloon->addComponent<RemoveEntityEvent>();
            else {
                damageBloon(bloon, damageEvent.shot, damageEvent.damage, gameData, newBloons);
                bloon->removeComponent<DamageEvent>();
            }
        }
    }
    layers[BLOONS_LAYER] += newBloons;
}
