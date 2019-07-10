//
// Created by Ido Mozes on 10/07/2019.
//

#ifndef SDL_GAME_RANGE_H
#define SDL_GAME_RANGE_H

#include "../Component.h"

class Range : public Component {
public:
    float range;
    static constexpr ComponentType type = ComponentType::RANGE;

    explicit Range(float range) : range(range) {}

    ~Range() override = default;
};

#endif //SDL_GAME_RANGE_H
