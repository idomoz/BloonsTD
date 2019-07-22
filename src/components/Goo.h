//
// Created by Ido Mozes on 18/07/2019.
//

#ifndef SDL_GAME_GOO_H
#define SDL_GAME_GOO_H

#include "../Component.h"

class Goo : public Component {
public:
    int kind;
    int ttl;
    int interval;
    int timetoRecharge;
    bool soak = false;
    int damage = 1;
    float stickness = 0.6;
    static constexpr ComponentType type = ComponentType::GOO;

    Goo(int kind, int ttl, int interval=60) : kind(kind), ttl(ttl), timetoRecharge(interval),interval(interval) {}
};

#endif //SDL_GAME_GOO_H
