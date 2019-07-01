//
// Created by Ido Mozes on 23/06/2019.
//

#ifndef SDL2_GAME_RENDERSYSTEM_H
#define SDL2_GAME_RENDERSYSTEM_H

#include <iostream>
#include "../components/Visibility.h"
#include "../System.h"

class RenderSystem : public System {
    static uint64_t mask;
    SDL_Window *window;
    SDL_Renderer *renderer;
public:
    explicit RenderSystem(bool fullscreen);

    ~RenderSystem();

    SDL_Renderer *getRenderer() { return renderer; }

    void update(std::vector<std::unique_ptr<Entity>> &entities) override;
};

#endif //SDL2_GAME_RENDERSYSTEM_H
