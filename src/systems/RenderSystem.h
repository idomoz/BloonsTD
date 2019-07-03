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
    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;
public:
    RenderSystem() { SDL_Init(SDL_INIT_EVERYTHING); }

    ~RenderSystem();

    void init(GameData &gameData);

    SDL_Renderer *getRenderer() { return renderer; }

    void update(std::vector<std::shared_ptr<Entity>> &entities , GameData & gameData) override;
};

#endif //SDL2_GAME_RENDERSYSTEM_H
