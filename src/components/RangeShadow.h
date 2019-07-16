//
// Created by Ido Mozes on 10/07/2019.
//

#ifndef SDL_GAME_RANGESHADOW_H
#define SDL_GAME_RANGESHADOW_H

#include "../Component.h"
#include <memory>

class RangeShadow : public Component {

public:
    EntityP entity;
    static constexpr ComponentType type = ComponentType::RANGE_SHADOW;

    RangeShadow(EntityP &entity) : entity(entity) {};

};

#endif //SDL_GAME_RANGESHADOW_H
