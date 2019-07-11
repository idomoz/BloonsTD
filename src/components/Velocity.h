//
// Created by Ido Mozes on 02/07/2019.
//

#ifndef SDL_GAME_VELOCITY_H
#define SDL_GAME_VELOCITY_H

#include <cmath>
#include "../Physics.h"

#include "../Component.h"

class Velocity : public Component {

public:
    Point value;
    static constexpr ComponentType type = ComponentType::VELOCITY;

    Velocity(float x, float y);

    ~Velocity() override = default;


    void setVelocity(float x, float y) {
        value={x,y};
    }

    void changeVelocity(float deltaX, float deltaY) {
        value.X += deltaX;
        value.Y += deltaY;
    }

    void turnDirection(float deltaRad);

    void setDirection(float rad);

    void setSpeed(float speed);

    void changeSpeed(float deltaSpeed);

};

class Acceleration : public Velocity {
public:
    static constexpr ComponentType type = ComponentType::ACCELERATION;

    Acceleration(float x, float y) : Velocity(x, y) {}
};

#endif //SDL_GAME_VELOCITY_H
