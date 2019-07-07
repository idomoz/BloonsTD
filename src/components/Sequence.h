//
// Created by Ido Mozes on 07/07/2019.
//

#ifndef SDL_GAME_SEQUENCE_H
#define SDL_GAME_SEQUENCE_H

#include <cmath>
#include "../Component.h"

#define SEQUENCE_FINISHED (-1)

class Sequence : public Component {
    int amount;
    float interval;
    float timeToRecharge;
    int amountReady;

public:
    static ComponentType getComponentType() { return ComponentType::SEQUENCE; }

    Sequence(Entity *entity, int amount, float interval, float delay) : Component(entity), amount(amount),
                                                                        interval(interval),
                                                                        timeToRecharge(delay ? delay : interval),
                                                                        amountReady(delay == 0) {}

    int getAmountReady();
};

#endif //SDL_GAME_SEQUENCE_H
