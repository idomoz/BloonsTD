//
// Created by Ido Mozes on 18/07/2019.
//

#ifndef SDL_GAME_LOADLEVELSYSTEM_H
#define SDL_GAME_LOADLEVELSYSTEM_H
#include "boost/filesystem.hpp"
#include "../System.h"

struct Sequence_S {
    int delay;
    int kind;
    int amount;
    int bps;
    char regrow;
    char camo;
    char fortified;
};

class LoadLevelSystem : public System {
public:
    void update(Entities *layers, GameData &gameData) override;
};

#endif //SDL_GAME_LOADLEVELSYSTEM_H
