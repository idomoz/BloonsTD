//
// Created by Ido Mozes on 02/07/2019.
//

#ifndef SDL_GAME_VELOCITY_H
#define SDL_GAME_VELOCITY_H

#include <cmath>


#include "../Component.h"

class Velocity : public Component {
    float X, Y;

    inline void getAlphaAndR(float &alpha, float &R) {
        alpha = atan2f(Y, X);
        R = X / sinf(alpha);
    }

    inline void setXAndY(float alpha, float R) {
        X = R * cosf(alpha);
        Y = R * sinf(alpha);
    }

public:
    static constexpr ComponentType type = ComponentType::VELOCITY;

    Velocity(float x, float y);

    ~Velocity() override = default;

    float getX() { return X; }

    float getY() { return Y; }

    void setVelocity(float x, float y) {
        X = x;
        Y = y;
    }

    void changeVelocity(float deltaX, float deltaY) {
        X += deltaX;
        Y += deltaY;
    }

    void turnDirection(float deltaDeg);

    void setDirection(float deg);

    void setSpeed(float speed);

    void changeSpeed(float deltaSpeed);

};

class Acceleration : public Velocity {
public:
    static constexpr ComponentType type = ComponentType::ACCELERATION;

    Acceleration(float x, float y) : Velocity(x, y) {}
};

#endif //SDL_GAME_VELOCITY_H
