//
// Created by Ido Mozes on 11/07/2019.
//

#ifndef SDL_GAME_SHOTSSPAWNSYSTEM_H
#define SDL_GAME_SHOTSSPAWNSYSTEM_H
#include <vector>
#include <iterator>
#include "../System.h"

class ShotsSpawnSystem : public System {
public:
    void update(Entities *layers, GameData &gameData) override;
};

#endif //SDL_GAME_SHOTSSPAWNSYSTEM_H
