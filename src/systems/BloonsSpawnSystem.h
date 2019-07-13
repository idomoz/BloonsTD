//
// Created by Ido Mozes on 07/07/2019.
//

#ifndef SDL_GAME_BLOONSSPAWNSYSTEM_H
#define SDL_GAME_BLOONSSPAWNSYSTEM_H

#include <vector>
#include <iterator>
#include "../System.h"

class BloonsSpawnSystem : public System {
public:
    void update(Entities *layers, GameData &gameData) override;
};

#endif //SDL_GAME_BLOONSSPAWNSYSTEM_H
