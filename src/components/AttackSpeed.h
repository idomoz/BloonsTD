//
// Created by Ido Mozes on 14/07/2019.
//

#ifndef SDL_GAME_ATTACKSPEED_H
#define SDL_GAME_ATTACKSPEED_H

#include <cmath>
#include "../Component.h"

class AttackSpeed : public Component {
    float interval;
    float timeToRecharge = 0;
public:
    static constexpr ComponentType type = ComponentType::ATTACK_SPEED;

    explicit AttackSpeed(float amountPerSecond) : interval(60 / amountPerSecond) {}
    void recharge();
    int getAmountReady();
};

#endif //SDL_GAME_ATTACKSPEED_H
