//
// Created by Ido Mozes on 02/07/2019.
//

#ifndef SDL_GAME_POSITION_H
#define SDL_GAME_POSITION_H

#include "../Component.h"

class Position : public Component {
    float X, Y;
public:
    static constexpr ComponentType type = ComponentType::POSITION;

    Position(Entity *entity, float x, float y) : Component(entity), X(x), Y(y) {};

    ~Position() override= default;

    float getX() { return X; }

    float getY() { return Y; }

    void setPosition(float x, float y) {
        X = x;
        Y = y;
    }

    void changePosition(float deltaX, float deltaY) {
        X += deltaX;
        Y += deltaY;
    }
};

#endif //SDL_GAME_POSITION_H
