//
// Created by Ido Mozes on 02/07/2019.
//

#include "Velocity.h"

Velocity::Velocity(Entity *entity, float x, float y) : Component(entity), X(x), Y(y) {}



void Velocity::turnDirection(float deltaDeg) {
    float alpha, R;
    getAlphaAndR(alpha, R);
    setXAndY(alpha + deltaDeg, R);
}

void Velocity::setDirection(float deg) {
    float alpha, R;
    getAlphaAndR(alpha, R);
    setXAndY(deg, R);
}

void Velocity::changeSpeed(float deltaSpeed) {
    float alpha, R;
    getAlphaAndR(alpha, R);
    setXAndY(alpha, R + deltaSpeed);
}

void Velocity::setSpeed(float speed) {
    float alpha, R;
    getAlphaAndR(alpha, R);
    setXAndY(alpha, speed);
}

