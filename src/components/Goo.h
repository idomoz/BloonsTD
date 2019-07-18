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
    int timetoRecharge = 60;
    static constexpr ComponentType type = ComponentType::GOO;

    Goo(int kind, int ttl) : kind(kind), ttl(ttl) {}
};

#endif //SDL_GAME_GOO_H
