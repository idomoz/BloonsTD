//
// Created by Ido Mozes on 20/07/2019.
//

#ifndef SDL_GAME_UPGRADES_H
#define SDL_GAME_UPGRADES_H

#include <vector>
#include <tuple>
#include <array>
#include "../Component.h"
#include "SDL.h"

enum UpgradeTypes {
    RANGE_UPGRADE,
    PIERCE_UPGRADE,
    DISTANCE_UPGRADE,
    DAMAGE_UPGRADE,
    ATTACK_SPEED_UPGRADE,
    SPREAD_UPGRADE,
    CAMO_UPGRADE,
    ADD_GOO_UPGRADE,
    GOO_KIND_UPGRADE,
    GOO_SOAK_UPGRADE,
    GOO_DURATION_UPGRADE,
    GOO_STICKINESS_UPGRADE,
    CORROSIVE_INTERVAL_UPGRADE,
    CORROSIVE_DAMAGE_UPGRADE,
    SHOTS_AMOUNT_UPGRADE,
    SHOT_KIND_UPGRADE,
    MOAB_CLASS_AFFECTING_UPGRADE,
};

class Upgrade {
public:
    std::vector<std::tuple<int, float>> improvements;
    SDL_Surface *surface;
    SDL_Texture *texture;
    SDL_Texture *lockedTexture;
    std::string name;
    int cost;
    bool locked = false;

    Upgrade(const std::string &name, int cost, SDL_Surface *surface, SDL_Texture *texture, SDL_Texture *lockedTexture,
            std::initializer_list<std::pair<int, float>> improvements) : name(name), cost(cost), surface(surface),
                                                                         texture(texture),
                                                                         lockedTexture(lockedTexture) {
        for (auto[kind, value]: improvements)
            this->improvements.emplace_back(kind, value);
    }
};

class UpgradeP : public Component {
public:
    Upgrade *value;
    static constexpr ComponentType type = ComponentType::UPGRADE_P;
    std::string name;

    UpgradeP(Upgrade *value) : value(value), name(value->name) {}
};

class Upgrades : public Component {
public:
    std::array<std::vector<Upgrade>, 3> paths;
    int selectedPathUpgrades[3];
    int chosenPath = -1;
    static constexpr ComponentType type = ComponentType::UPGRADES;

};

#endif //SDL_GAME_UPGRADES_H
