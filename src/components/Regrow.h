//
// Created by Ido Mozes on 18/07/2019.
//

#ifndef SDL_GAME_REGROW_H
#define SDL_GAME_REGROW_H
#include "../Component.h"

class Regrow: public Component{
public:
    int kind;
    int regrowTime = 60;
    static constexpr ComponentType type = ComponentType::REGROW;
    explicit Regrow(int kind):kind(kind){}
    Regrow(const Regrow & other):kind(other.kind){}
};
#endif //SDL_GAME_REGROW_H
