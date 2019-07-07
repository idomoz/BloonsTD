//
// Created by Ido Mozes on 23/06/2019.
//

#ifndef SDL2_GAME_RENDERSYSTEM_H
#define SDL2_GAME_RENDERSYSTEM_H

#include <iostream>
#include "../components/Visibility.h"
#include "../System.h"
#include "../GameData.h"

class RenderSystem : public System {
    static uint64_t mask;

public:
    RenderSystem() { SDL_Init(SDL_INIT_EVERYTHING); }


    void init(GameData &gameData);

    void update(std::vector<std::shared_ptr<Entity>> &entities , GameData & gameData) override;
};

#endif //SDL2_GAME_RENDERSYSTEM_H
