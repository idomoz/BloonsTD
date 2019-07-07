//
// Created by Ido Mozes on 07/07/2019.
//

#ifndef SDL_GAME_SPAWNSYSTEM_H
#define SDL_GAME_SPAWNSYSTEM_H

#include <vector>
#include <iterator>
#include "../System.h"
#include "../components/Sequence.h"
#include "../components/Speed.h"
#include "../components/Kind.h"
#include "../components/Position.h"
#include "../components/Visibility.h"
#include "../components/PathIndex.h"

class SpawnSystem : public System {
    static uint64_t mask;
public:
    void update(std::vector<std::shared_ptr<Entity>> *layers, GameData &gameData) override;
};

#endif //SDL_GAME_SPAWNSYSTEM_H
