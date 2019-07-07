//
// Created by Ido Mozes on 23/06/2019.
//

#include "RenderSystem.h"
#include "../components/Visibility.h"
#include "../components/Position.h"


uint64_t RenderSystem::mask = createMask({ComponentType::VISIBILITY});


void RenderSystem::init(GameData &gameData) {
    if (gameData.window != nullptr)
        SDL_DestroyWindow(gameData.window);
    if (gameData.renderer != nullptr)
        SDL_DestroyRenderer(gameData.renderer);
    gameData.window = SDL_CreateWindow("BloonsTD", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                       (685 + 400) * gameData.mapScale,
                                       511 * gameData.mapScale, gameData.fullscreen ? SDL_WINDOW_FULLSCREEN : 0);
    gameData.renderer = SDL_CreateRenderer(gameData.window, -1, 0);
    SDL_SetRenderDrawColor(gameData.renderer, 255, 255, 255, 255);
}

void RenderSystem::update(std::vector<std::shared_ptr<Entity>> *layers, GameData &gameData) {
    SDL_RenderClear(gameData.renderer);
    for (int i = 0; i < N_LAYERS; ++i) {
        for (auto &entity: layers[i]) {
            if (entity->hasComponents(mask)) {
                auto &visibility = entity->getComponent<Visibility>();
                SDL_Rect r = {int(visibility.getDstRect()->x * gameData.mapScale),
                              int(visibility.getDstRect()->y * gameData.mapScale),
                              int(visibility.getDstRect()->w * gameData.mapScale),
                              int(visibility.getDstRect()->h * gameData.mapScale)};
                if (entity->hasComponents(createMask({ComponentType::POSITION}))) {
                    if (entity->hasComponents(createMask({ComponentType::POSITION}))) {
                        auto &position = entity->getComponent<Position>();
                        r.x = int(position.getX() * gameData.mapScale);
                        r.y = int(position.getY() * gameData.mapScale);
                    }
                    r.x -= int(r.w / 2.0);
                    r.y -= int(r.h / 2.0);
                    r.x += int(150 * gameData.mapScale);
                }
                SDL_RenderCopy(gameData.renderer, visibility.getTexture(), nullptr, &r);
            }
        }
    }
    SDL_RenderPresent(gameData.renderer);
}


