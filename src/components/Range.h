//
// Created by Ido Mozes on 10/07/2019.
//

#ifndef SDL_GAME_RANGE_H
#define SDL_GAME_RANGE_H

#include "../Component.h"

class Range : public Component {
public:
    float value;
    static constexpr ComponentType type = ComponentType::RANGE;

    explicit Range(float value) : value(value) {}

    ~Range() override = default;
};

#endif //SDL_GAME_RANGE_H
