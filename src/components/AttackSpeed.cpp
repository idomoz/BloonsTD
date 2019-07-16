//
// Created by Ido Mozes on 14/07/2019.
//
#include "AttackSpeed.h"

void AttackSpeed::recharge() {
    timeToRecharge = std::fmaxf(timeToRecharge - 1, 0);
}

int AttackSpeed::getAmountReady() {
    int amountReady = 0;
    if (timeToRecharge == 1) {
        timeToRecharge = interval;
        amountReady = 1;
    } else if (timeToRecharge > 1) {
        timeToRecharge -= 1;
    } else {
        amountReady = 1 + int((1 - timeToRecharge) / interval);
        timeToRecharge = interval - fmodf((1 - timeToRecharge), interval);
        if (timeToRecharge == 0)
            timeToRecharge = interval;
    }
    return amountReady;
}