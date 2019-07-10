//
// Created by Ido Mozes on 20/06/2019.
//

#ifndef SDL2_GAME_COMPONENT_H
#define SDL2_GAME_COMPONENT_H
struct Point {
    short X;
    short Y;
};

class Entity;

enum ComponentType {
    VISIBILITY, POSITION, VELOCITY, SPEED, ACCELERATION, PATH_INDEX, HEALTH, KIND, TYPE,RANGE, DAMAGE, PIERCE, SPREAD, SEQUENCE, ACTION, DRAGGABLE,
    MOVE_ENTITY_EVENT, REMOVE_ENTITY_EVENT,
    LENGTH

};

class Component {
public:
     Component() = default;

    virtual ~Component() = default;

};

#endif //SDL2_GAME_COMPONENT_H
