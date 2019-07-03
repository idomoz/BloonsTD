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
    VISIBILITY, POSITION, VELOCITY,SPEED, ACCELERATION, PATH_INDEX, HEALTH, KIND, TYPE, DAMAGE, PIERCE, SPREAD,
    LENGTH

};

class Component {
    Entity *entity;
public:
    explicit Component(Entity *entity) : entity(entity) {};

    ~Component() = default;

    Entity *getEntity() { return entity; }

};

#endif //SDL2_GAME_COMPONENT_H
