//
// Created by Ido Mozes on 07/07/2019.
//

#ifndef SDL_GAME_SPAWNSYSTEM_H
#define SDL_GAME_SPAWNSYSTEM_H

#include <vector>
#include <iterator>
#include "../System.h"

class SpawnSystem : public System {
public:
    void update(std::vector<std::shared_ptr<Entity>> *layers, GameData &gameData) override;
};

#endif //SDL_GAME_SPAWNSYSTEM_H
