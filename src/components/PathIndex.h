//
// Created by Ido Mozes on 03/07/2019.
//

#ifndef SDL_GAME_PATHINDEX_H
#define SDL_GAME_PATHINDEX_H

#include "../Component.h"


class PathIndex : public Component {
public:
    int index;

    static ComponentType getComponentType() { return ComponentType::PATH_INDEX; }

    PathIndex(Entity *entity, int index) : Component(entity), index(index) {}
};

#endif //SDL_GAME_PATHINDEX_H
