//
// Created by Ido Mozes on 20/06/2019.
//

#ifndef SDL2_GAME_COMPONENT_H
#define SDL2_GAME_COMPONENT_H

#include <memory>

struct Point {
    float X;
    float Y;
};

class Entity;

typedef std::shared_ptr<Entity> EntityP;
enum ComponentType {
    VISIBILITY,
    POSITION,
    VELOCITY,
    SPEED,
    ATTACK_SPEED,
    ACCELERATION,
    PATH_INDEX,
    LIVES,
    KIND,
    SHOT_KIND,
    TYPE,
    RANGE,
    DAMAGE,
    PIERCE,
    SPREAD,
    SHOTS_AMOUNT,
    DISTANCE,
    SEQUENCE,
    ACTION,
    DRAGGABLE,
    RANGE_SHADOW,
    STRATEGY,
    REGROW,
    CAMO,
    FORTIFIED,
    GOO,
    COST,
    POPPED_BLOONS,
    UPGRADES,
    UPGRADE_P,
    SEEN,
    MOAB_CLASS_AFFECTING,

    MOVE_ENTITY_EVENT,
    REMOVE_ENTITY_EVENT,
    DAMAGE_EVENT,

    LENGTH
};

class Component {
public:
    Component() = default;

    virtual ~Component() = default;

};

#endif //SDL2_GAME_COMPONENT_H
