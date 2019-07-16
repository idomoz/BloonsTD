//
// Created by Ido Mozes on 07/07/2019.
//

#include "Sequence.h"


int Sequence::getAmountReady() {
    if (amount == 0)
        return SEQUENCE_FINISHED;
    int amountToReturn;
    if (amountReady <= amount) {
        amount -= amountReady;
        amountToReturn = amountReady;
    } else {
        amountToReturn = amount;
        amount = 0;
    }
    amountReady = 0;
    if (timeToRecharge == 1) {
        timeToRecharge = interval;
        amountReady = 1;
    } else if (timeToRecharge > 1) {
        timeToRecharge -= 1;
    } else {
        amountReady = 1 + int((1 - timeToRecharge) / interval);
        timeToRecharge =  interval - fmodf((1 - timeToRecharge), interval);
        if(timeToRecharge == 0)
            timeToRecharge = interval;
    }
    return amountToReturn;
}
