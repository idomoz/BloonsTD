//
// Created by Ido Mozes on 23/06/2019.
//

#include "RenderSystem.h"
#include "../components/Visibility.h"


uint64_t RenderSystem::mask = createMask({ComponentType::VISIBILITY});

RenderSystem::RenderSystem(bool fullscreen) {
    SDL_Init(SDL_INIT_EVERYTHING);
    window = SDL_CreateWindow("BloonsTD", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720,
                              fullscreen ? SDL_WINDOW_FULLSCREEN : 0);
    renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
}

RenderSystem::~RenderSystem() {
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
}

void RenderSystem::update(std::vector<std::unique_ptr<Entity>>& entities) {
    SDL_RenderClear(renderer);
    for (auto &entity: entities) {
        if (entity->hasComponents(mask)) {
            auto &v = entity->getComponent<Visibility>();
            SDL_RenderCopy(renderer, v.getTexture(), v.getSrcRect(), v.getDstRect());
        }
    }
    SDL_RenderPresent(renderer);
}


