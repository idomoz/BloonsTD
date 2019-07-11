//
// Created by Ido Mozes on 02/07/2019.
//

#include "Velocity.h"

Velocity::Velocity(float x, float y) : value{x, y} {}


void Velocity::turnDirection(float deltaRad) {
    auto[alpha, R] = cartesianToPolar(value.X, value.Y);
    std::tie(value.X, value.Y) = polarToCartesian(alpha + deltaRad, R);
}

void Velocity::setDirection(float rad) {
    auto[_, R] = cartesianToPolar(value.X, value.Y);
    std::tie(value.X, value.Y) = polarToCartesian(rad, R);
}

void Velocity::changeSpeed(float deltaSpeed) {
    auto[alpha, R] = cartesianToPolar(value.X, value.Y);
    std::tie(value.X, value.Y) = polarToCartesian(alpha, R + deltaSpeed);
}

void Velocity::setSpeed(float speed) {
    auto[alpha, _] = cartesianToPolar(value.X, value.Y);
    std::tie(value.X, value.Y) = polarToCartesian(alpha, speed);
}

