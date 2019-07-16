//
// Created by Ido Mozes on 02/07/2019.
//

#ifndef SDL_GAME_POSITION_H
#define SDL_GAME_POSITION_H

#include "../Component.h"

class Position : public Component {

public:
    Point value;
    static constexpr ComponentType type = ComponentType::POSITION;

    Position(float x, float y) : value{x, y} {}

    explicit Position(Point value) : value(value) {}


    void changePosition(float deltaX, float deltaY) {
        value.X += deltaX;
        value.Y += deltaY;
    }
};

#endif //SDL_GAME_POSITION_H
