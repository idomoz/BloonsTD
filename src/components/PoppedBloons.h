//
// Created by Ido Mozes on 15/07/2019.
//

#ifndef SDL_GAME_POPPEDBLOONS_H
#define SDL_GAME_POPPEDBLOONS_H
#include "../Component.h"
#include <unordered_set>
class PoppedBloons: public Component{
public:
    std::unordered_set<void *> value;
    static constexpr ComponentType type = ComponentType::POPPED_BLOONS;
};
#endif //SDL_GAME_POPPEDBLOONS_H
