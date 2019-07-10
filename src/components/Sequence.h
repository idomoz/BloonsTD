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

    Sequence(int amount, float interval, float delay) : amount(amount), interval(interval),
                                                        timeToRecharge(delay ? delay : interval),
                                                        amountReady(delay == 0) {}

    ~Sequence() override = default;

    int getAmountReady();
};

#endif //SDL_GAME_SEQUENCE_H
