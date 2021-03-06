//
// Created by Ido Mozes on 07/07/2019.
//

#ifndef SDL_GAME_SEQUENCE_H
#define SDL_GAME_SEQUENCE_H

#include <cmath>
#include "../Component.h"

constexpr int SEQUENCE_FINISHED = -1;

class Sequence : public Component {
    int amount;
    float interval;
    float timeToRecharge;
    int amountReady;

public:
    static constexpr ComponentType type = ComponentType::SEQUENCE;

    Sequence(int amount, float amountPerSecond, float delay=0) : amount(amount), interval(60/amountPerSecond),
                                                        timeToRecharge(delay ? delay : interval),
                                                        amountReady(delay == 0) {}


    int getAmountReady();
};

#endif //SDL_GAME_SEQUENCE_H
