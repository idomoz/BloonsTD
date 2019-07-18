//
// Created by Ido Mozes on 03/07/2019.
//

#ifndef SDL_GAME_PATHINDEX_H
#define SDL_GAME_PATHINDEX_H

#include "../Component.h"


class PathIndex : public Component {
public:
    int index;
    float progress;

    static constexpr ComponentType type = ComponentType::PATH_INDEX;

    explicit PathIndex(int index) : index(index), progress(index) {}

    PathIndex(int index, float progress) : index(index), progress(progress) {}
};

#endif //SDL_GAME_PATHINDEX_H
