//
// Created by Ido Mozes on 14/07/2019.
//

#ifndef SDL_GAME_FLOATCOMPONENTS_H
#define SDL_GAME_FLOATCOMPONENTS_H
#define FLOAT_COMPONENT(className, classType)  class className : public FloatComponent { public:  static constexpr ComponentType type = ComponentType::classType; using FloatComponent::FloatComponent; }

#include "../Component.h"

class FloatComponent : public Component {
public:
    float value;

    explicit FloatComponent(float value) : value(value) {}

};
FLOAT_COMPONENT(Range,RANGE);
FLOAT_COMPONENT(Speed,SPEED);
FLOAT_COMPONENT(Spread,SPREAD);
FLOAT_COMPONENT(Distance,DISTANCE);
#endif //SDL_GAME_FLOATCOMPONENTS_H
