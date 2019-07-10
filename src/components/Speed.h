//
// Created by Ido Mozes on 03/07/2019.
//

#ifndef SDL_GAME_SPEED_H
#define SDL_GAME_SPEED_H

#include "../Component.h"


class Speed : public Component {
public:
    float speed;

    static constexpr ComponentType type = ComponentType::SPEED;

    explicit Speed(float speed) :speed(speed) {}
    ~Speed() override = default;
};

#endif //SDL_GAME_SPEED_H
