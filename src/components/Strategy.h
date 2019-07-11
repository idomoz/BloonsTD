//
// Created by Ido Mozes on 11/07/2019.
//

#ifndef SDL_GAME_STRATEGY_H
#define SDL_GAME_STRATEGY_H
#include "../Component.h"

enum Strateies {
    FIRST, LAST, CLOSEST, STRONGEST
};

class Strategy : public Component {
public:
    Strateies value;
    static constexpr ComponentType type = ComponentType::STRATEGY;

    Strategy(Strateies value) : value(value) {}

    ~Strategy() override = default;
};

#endif //SDL_GAME_STRATEGY_H
