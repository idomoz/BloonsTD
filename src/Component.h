//
// Created by Ido Mozes on 20/06/2019.
//

#ifndef SDL2_GAME_COMPONENT_H
#define SDL2_GAME_COMPONENT_H


class Entity;

enum ComponentType {
    POSITION, VISIBILITY,
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
