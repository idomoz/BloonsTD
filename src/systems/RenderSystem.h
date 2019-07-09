//
// Created by Ido Mozes on 23/06/2019.
//

#ifndef SDL2_GAME_RENDERSYSTEM_H
#define SDL2_GAME_RENDERSYSTEM_H

#include <iostream>
#include "../System.h"
#include "../GameData.h"
#include "SDL2_gfxPrimitives.h"



class RenderSystem : public System {

public:
    RenderSystem() { SDL_Init(SDL_INIT_EVERYTHING); }


    void init(GameData &gameData);

    void update(std::vector<std::shared_ptr<Entity>> *layers , GameData & gameData) override;
};

#endif //SDL2_GAME_RENDERSYSTEM_H
