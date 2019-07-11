//
// Created by Ido Mozes on 11/07/2019.
//

#ifndef SDL_GAME_PHYSICS_H
#define SDL_GAME_PHYSICS_H

#include <tuple>
#include <cmath>
#include "Component.h"

inline std::tuple<float, float> cartesianToPolar(float X, float Y) {
    float alpha = atan2f(Y, X);
    float R = X / sinf(alpha);
    return std::make_tuple(alpha, R);
}

inline std::tuple<float, float> polarToCartesian(float alpha, float R) {
    return std::make_tuple(R * cosf(alpha), R * sinf(alpha));
}

inline float twoPointsDistance(Point point1, Point point2) {
    return sqrtf(powf(point1.X - point2.X, 2) +
                 powf(point1.Y - point2.Y, 2));
}

inline float twoPointsAngle(Point point1, Point point2) {
    return atan2f(point2.Y - point1.Y, point2.X - point1.X);
}

inline float radToDeg(float radians) {
    return radians * (180.0 / 3.141592653589793238463);
}

#endif //SDL_GAME_PHYSICS_H
