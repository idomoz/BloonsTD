//
// Created by Ido Mozes on 23/06/2019.
//

#include "RenderSystem.h"
#include "../components/Visibility.h"
#include "../components/Position.h"


void RenderSystem::init(GameData &gameData) {
    if (gameData.window != nullptr)
        SDL_DestroyWindow(gameData.window);
    if (gameData.renderer != nullptr)
        SDL_DestroyRenderer(gameData.renderer);
    gameData.window = SDL_CreateWindow("BloonsTD", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                       (MAP_WIDTH + SIDEBAR_WIDTH + MENU_WIDTH) * gameData.mapScale,
                                       MAP_HEIGHT * gameData.mapScale, gameData.fullscreen ? SDL_WINDOW_FULLSCREEN : 0);
    gameData.renderer = SDL_CreateRenderer(gameData.window, -1, 0);
    SDL_SetRenderDrawColor(gameData.renderer, 255, 255, 255, 255);
}

void RenderSystem::update(std::vector<std::shared_ptr<Entity>> *layers, GameData &gameData) {

    SDL_RenderClear(gameData.renderer);
    for (int i = 0; i < N_LAYERS; ++i) {
        for (auto &entity: layers[i]) {
            if (auto visibilityP = entity->getComponent<Visibility>()) {
                auto &visibility = *visibilityP;
                auto positionP = entity->getComponent<Position>();
                SDL_Rect r = {int(visibility.getDstRect()->x * gameData.mapScale),
                              int(visibility.getDstRect()->y * gameData.mapScale),
                              int(visibility.getDstRect()->w * gameData.mapScale),
                              int(visibility.getDstRect()->h * gameData.mapScale)};
                if (positionP) {
                    auto &position = *positionP;
                    r.x = int((position.getX() + SIDEBAR_WIDTH) * gameData.mapScale -r.w / 2.0);
                    r.y = int(position.getY() * gameData.mapScale -r.h / 2.0);
                }
                SDL_RenderCopy(gameData.renderer, visibility.getTexture(), nullptr, &r);
                if (visibility.getRadios() != NO_RADIOS) {
                    int entityX, entityY;
                    if (positionP) {
                        entityX = r.x;
                        entityY = r.y;
                    } else {
                        entityX = r.x + int((visibility.getDstRect()->w / 2.0) * gameData.mapScale);
                        entityY = r.y + int((visibility.getDstRect()->h / 2.0) * gameData.mapScale);
                    }
                    auto draggableP = entity->getComponent<Draggable>();
                    bool isRed = draggableP ? !draggableP->isPlaceable : false;
                    filledCircleRGBA(gameData.renderer, entityX, entityY, visibility.getRadios(), isRed ? 255 : 0, 0, 0,
                                     100);
                    aacircleRGBA(gameData.renderer, entityX, entityY, visibility.getRadios(), isRed ? 255 : 0, 0, 0,
                                 150);

                }
            }
        }
    }

    SDL_RenderPresent(gameData.renderer);
}


