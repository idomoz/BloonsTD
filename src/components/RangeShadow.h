//
// Created by Ido Mozes on 10/07/2019.
//

#ifndef SDL_GAME_RANGESHADOW_H
#define SDL_GAME_RANGESHADOW_H

#include "../Component.h"
#include <memory>

class RangeShadow : public Component {

public:
    std::shared_ptr<Entity> entity;
    static constexpr ComponentType type = ComponentType::RANGE_SHADOW;

    RangeShadow(std::shared_ptr<Entity> &entity) : entity(entity) {};

    ~RangeShadow() override = default;
};

#endif //SDL_GAME_RANGESHADOW_H
