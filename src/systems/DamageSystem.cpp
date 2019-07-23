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

EntityP
spawnBloon(Entities &newBloons, GameData &gameData, int kind, EntityP &shot, int lives, float progress,
           int regrow, bool camo, bool fortified, Goo *gooP) {
    EntityP newBloon(new Entity());
    if (shot)
        shot->getComponent<PoppedBloons>()->value.emplace(newBloon.get());
    newBloon->addComponent<Type>(BLOON_T);
    newBloon->addComponent<Kind>(kind);
    newBloon->addComponent<Lives>(lives);
    newBloon->addComponent<Position>(gameData.startingPoint);
    newBloon->addComponent<PathIndex>(0, progress);
    if (regrow)
        newBloon->addComponent<Regrow>(regrow);
    if (camo)
        newBloon->addComponent<Camo>();
    if (fortified)
        newBloon->addComponent<Fortified>();
    if (gooP and gooP->soak)
        newBloon->addComponent<Goo>(*gooP);
    auto [texture,surface] = gameData.getTexture(getSurfaceName(newBloon));
    newBloon->addComponent<Range>(std::max(surface->w / 6, surface->h / 6));
    newBloon->addComponent<Visibility>(texture, surface, SDL_Rect{0, 0, surface->w / 3, 0});
    newBloons.emplace_back(newBloon);
    return newBloon;
}

void damageBloon(EntityP &bloon, EntityP &shot, int damage, GameData &gameData, Entities &newBloons) {
    if (damage == 0)
        return;
    auto &lives = bloon->getComponent<Lives>()->value;
    if (damage >= lives) {
        gameData.cash += getBloonProperty<YIELD>(bloon);
        bloon->addComponent<RemoveEntityEvent>();
        return;
    }
    auto &kind = bloon->getComponent<Kind>()->value;
    int shotKind = 0;
    if (shot)
        shotKind = shot->getComponent<Kind>()->value;
    float progress = bloon->getComponent<PathIndex>()->progress;
    SDL_Surface *surface;
    SDL_Texture * texture;
    auto &visibility = *bloon->getComponent<Visibility>();
    if (kind > PINK_BLOON) {
        if (kind >= CERAMIC_BLOON and !didBloonPop(bloon, lives, damage, getBloonProperty<MIN_LIVES>(bloon))) {
            std::tie(texture,surface) = gameData.getTexture(getSurfaceName(bloon));
            visibility.reloadTexture(texture, surface);
            return;
        }
        if (kind < CERAMIC_BLOON and !didBloonPop(bloon, lives, damage))
            return;
    }
    if (kind > PINK_BLOON) {
        gameData.cash += 1;
        bloon->addComponent<RemoveEntityEvent>();
    }
    auto[regrowP, camoP, fortifiedP, gooP] = bloon->getComponentsP<Regrow, Camo, Fortified, Goo>();
    switch (kind) {
        case RED_BLOON:
            lives -= 1;
            break;
        case BLUE_BLOON:
        case GREEN_BLOON:
        case YELLOW_BLOON:
        case PINK_BLOON: {
            lives -= damage;
            if (fortifiedP and damage == 1 and lives % 2 == 1)
                break;
            if (gooP and !gooP->soak)
                bloon->removeComponent<Goo>();
            gameData.cash += kind - ((fortifiedP ? lives / 2 : lives) - 1);
            kind = (fortifiedP ? lives / 2 : lives) - 1;
            std::tie(texture,surface) = gameData.getTexture(getSurfaceName(bloon));
            visibility.setDstRect(SDL_Rect{0, 0, surface->w / 3, 0});
            visibility.reloadTexture(texture, surface);
            if (regrowP)
                regrowP->regrowTime = 60;
            bloon->getComponent<Range>()->value = std::max(surface->w / 6, surface->h / 6);
            break;
        }
        case PURPLE_BLOON:
        case WHITE_BLOON:
        case BLACK_BLOON: {
            for (int i = 0; i < 2; ++i) {
                EntityP newBloon = spawnBloon(newBloons, gameData, PINK_BLOON, shot, lives / 2,
                                              i == 0 ? std::fmaxf(0, progress - 10) : std::fminf(
                                                      gameData.path.size() - 1, progress + 10),
                                              regrowP ? regrowP->kind : 0, camoP, false, gooP);
                damageBloon(newBloon, shot,damage, gameData, newBloons);
            }
            break;
        }
        case ZEBRA_BLOON: {
            for (int i = 0; i < 2; ++i) {
                EntityP newBloon = spawnBloon(newBloons, gameData, i == 0 ? BLACK_BLOON : WHITE_BLOON, shot, lives / 2,
                                              i == 0 ? std::fmaxf(0, progress - 12) : std::fminf(
                                                      gameData.path.size() - 1, progress + 12),
                                              regrowP ? regrowP->kind : 0, camoP, false, gooP);
                damageBloon(newBloon, shot,damage, gameData, newBloons);
            }
            break;
        }
        case LEAD_BLOON:
        case RAINBOW_BLOON:
        case CERAMIC_BLOON: {
            for (int i = 0; i < 2; ++i) {
                EntityP newBloon = spawnBloon(newBloons, gameData, kind - 1, shot, lives / 2,
                                              i == 0 ? std::fmaxf(0, progress - 14) : std::fminf(
                                                      gameData.path.size() - 1, progress + 14),
                                              regrowP ? regrowP->kind : 0, camoP, false, gooP);
                damageBloon(newBloon, shot, damage, gameData, newBloons);
            }
            break;
        }
        case MOAB:
        case BFB:
        case ZOMG:
        case DDT: {
            for (int i = 0; i < 4; ++i) {
                EntityP newBloon = spawnBloon(newBloons, gameData, kind == DDT ? CERAMIC_BLOON : kind - 1,
                                              shot, lives / 4,
                                              (i < 2 ? std::fmaxf(0, progress - 20 + 10 * (i % 2)) : std::fminf(
                                                      gameData.path.size() - 1, progress + 20 - 10 * (i % 2))),
                                              kind == DDT ? CERAMIC_BLOON : 0, kind == DDT, fortifiedP, nullptr);
                damageBloon(newBloon, shot,damage, gameData, newBloons);
            }
            break;
        }
        case BAD: {
            for (int i = 0; i < 5; ++i) {
                EntityP newBloon(new Entity());
                shot->getComponent<PoppedBloons>()->value.emplace(newBloon.get());
                newBloon->addComponent<Type>(BLOON_T);
                newBloon->addComponent<Kind>(i < 2 ? ZOMG : DDT);
                if (fortifiedP)
                    newBloon->addComponent<Fortified>();
                if (i >= 2)
                    newBloon->addComponent<Camo>();
                newBloon->addComponent<Lives>(getBloonProperty<TOTAL_LIVES>(newBloon));
                newBloon->addComponent<Position>(gameData.startingPoint);
                newBloon->addComponent<PathIndex>(0);
                newBloon->getComponent<PathIndex>()->progress =
                        i < 2 ? std::fmaxf(0, progress - 20 + 10 * (i % 2)) : std::fminf(
                                gameData.path.size() - 1, progress + 30 - 10 * (i % 3));
                std::tie(texture,surface) = gameData.getTexture(getSurfaceName(newBloon));
                newBloon->addComponent<Range>(std::max(surface->w / 6, surface->h / 6));
                newBloon->addComponent<Visibility>(texture, surface, SDL_Rect{0, 0, surface->w / 3, 0});
                newBloons.emplace_back(newBloon);
                damageBloon(newBloon, shot,damage, gameData, newBloons);
            }
            break;
        }
    }
}

void DamageSystem::update(Entities *layers, GameData &gameData) {
    if (!gameData.levelRunning)
        return;
    Entities newBloons;
    for (auto &bloon: layers[BLOONS_LAYER]) {
        if (auto damageEventP = bloon->getComponent<DamageEvent>()) {
            auto &damageEvent = *damageEventP;
            auto &lives = bloon->getComponent<Lives>()->value;
            if (lives < damageEvent.damage) {
                gameData.cash += getBloonProperty<YIELD>(bloon);
                bloon->addComponent<RemoveEntityEvent>();
            } else {
                damageBloon(bloon, damageEvent.shot, damageEvent.damage, gameData, newBloons);
                bloon->removeComponent<DamageEvent>();
            }
        }
    }
    layers[BLOONS_LAYER] += newBloons;
}
