//
// Created by Ido Mozes on 23/06/2019.
//

#ifndef SDL2_GAME_SYSTEM_H
#define SDL2_GAME_SYSTEM_H

#include <initializer_list>
#include <vector>
#include <cmath>
#include "Entity.h"
#include "GameData.h"
#include "components/IntegerComponents.h"
#include "components/FloatComponents.h"
#include "components/FlagComponents.h"
#include "components/Sequence.h"
#include "components/Position.h"
#include "components/Visibility.h"
#include "components/PathIndex.h"
#include "components/Velocity.h"
#include "components/Draggable.h"
#include "components/Action.h"
#include "components/RangeShadow.h"
#include "components/AttackSpeed.h"
#include "components/DamageEvent.h"
#include "components/PoppedBloons.h"
#include "components/MoveEntityEvent.h"
#include "components/Regrow.h"
#include "components/Goo.h"
#include "components/Upgrades.h"

typedef std::vector<EntityP> Entities;

inline void
operator+=(Entities &originalVector, Entities &newVector) {
    originalVector.insert(originalVector.end(), std::make_move_iterator(newVector.begin()),
                          std::make_move_iterator(newVector.end()));
}

std::string getSurfaceName(EntityP &entity);

enum ReturnValue {
    TOTAL_LIVES, MIN_LIVES, SELF_LIVES, YIELD
};

float getSpeed(EntityP &entity);

template<ReturnValue returnValue>
int getBloonProperty(EntityP &entity) {
    int totalLives = 0;
    int selfLives = 0;
    int minLives = 0;
    int yield = 0;
    int additionalPreviousFortifiedLives = 0;
    auto fortifiedP = entity->getComponent<Fortified>();
    switch (entity->getComponent<Kind>()->value) {
        case RED_BLOON:
            yield = totalLives = selfLives = 1;
            break;
        case BLUE_BLOON:
            yield = totalLives = selfLives = 2;
            break;
        case GREEN_BLOON:
            yield = totalLives = selfLives = 3;
            break;
        case YELLOW_BLOON:
            yield = totalLives = selfLives = 4;
            break;
        case PINK_BLOON:
            yield = totalLives = selfLives = 5;
            break;
        case PURPLE_BLOON:
            selfLives = 1;
            yield = totalLives = 11;
            break;
        case WHITE_BLOON:
            selfLives = 1;
            yield = totalLives = 11;
            break;
        case BLACK_BLOON:
            selfLives = 1;
            yield = totalLives = 11;
            break;
        case ZEBRA_BLOON:
            selfLives = 1;
            yield = totalLives = 23;
            break;
        case LEAD_BLOON:
            selfLives = 1;
            yield = totalLives = 23;
            break;
        case RAINBOW_BLOON:
            selfLives = 1;
            yield = totalLives = 47;
            break;
        case CERAMIC_BLOON:
            selfLives = 10;
            yield = 95;
            totalLives = 104;
            break;
        case MOAB:
            additionalPreviousFortifiedLives = 40;
            yield = 381;
            selfLives = 200;
            totalLives = 616;
            break;
        case BFB:
            additionalPreviousFortifiedLives = 960;
            yield = 1525;
            selfLives = 1200;
            totalLives = 3164;
            break;
        case DDT:
            additionalPreviousFortifiedLives = 40;
            yield = 381;
            selfLives = 400;
            totalLives = 816;
            break;
        case ZOMG:
            additionalPreviousFortifiedLives = 8640;
            yield = 6101;
            selfLives = 4000;
            totalLives = 16656;
            break;
        case BAD:
            additionalPreviousFortifiedLives = 26600;
            yield = 13346;
            selfLives = 20000;
            totalLives = 55760;

            break;
    }
    minLives = totalLives - selfLives + 1;
    if (fortifiedP) {
        totalLives += additionalPreviousFortifiedLives + selfLives;
        selfLives *= 2;
        minLives += additionalPreviousFortifiedLives;
    }
    switch (returnValue) {
        case TOTAL_LIVES:
            return totalLives;
        case MIN_LIVES:
            return minLives;
        case SELF_LIVES:
            return selfLives;
        case YIELD:
            return yield;
    }
}

class System {
public:
    virtual void update(Entities *entities, GameData &gameData) = 0;
};

#endif //SDL2_GAME_SYSTEM_H
