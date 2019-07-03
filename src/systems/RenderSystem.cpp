//
// Created by Ido Mozes on 23/06/2019.
//

#include "RenderSystem.h"
#include "../components/Visibility.h"


uint64_t RenderSystem::mask = createMask({ComponentType::VISIBILITY});


RenderSystem::~RenderSystem() {
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
}
void RenderSystem::init(GameData &gameData){
    if (window != nullptr)
        SDL_DestroyWindow(window);
    if (renderer != nullptr)
        SDL_DestroyRenderer(renderer);
    window = SDL_CreateWindow("BloonsTD", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 700 *gameData.mapScale,
                              520 * gameData.mapScale, gameData.fullscreen ? SDL_WINDOW_FULLSCREEN : 0);
    renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
}
void RenderSystem::update(std::vector<std::shared_ptr<Entity>> &entities,  GameData & gameData) {
    SDL_RenderClear(renderer);
    for (auto &entity: entities) {
        if (entity->hasComponents(mask)) {
            auto &v = entity->getComponent<Visibility>();
            SDL_RenderCopy(renderer, v.getTexture(), nullptr, v.getDstRect());
        }
    }
    SDL_RenderPresent(renderer);
}


