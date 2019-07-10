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
    gameData.window = SDL_CreateWindow("BloonsTD", SDL_WINDOWPOS_CENTERED,
                                       SDL_WINDOWPOS_CENTERED, // NOLINT(hicpp-signed-bitwise)
                                       int((MAP_WIDTH + SIDEBAR_WIDTH + MENU_WIDTH) * gameData.mapScale),
                                       int(MAP_HEIGHT * gameData.mapScale),
                                       gameData.fullscreen ? SDL_WINDOW_FULLSCREEN : 0);
    gameData.renderer = SDL_CreateRenderer(gameData.window, -1, 0);
    SDL_SetRenderDrawColor(gameData.renderer, 255, 255, 255, 255);
}

void RenderSystem::update(Entities *layers, GameData &gameData) {

    SDL_RenderClear(gameData.renderer);
    for (int i = 0; i < N_LAYERS; ++i) {
        for (auto &entity: layers[i]) {
            auto rangeShadowP = entity->getComponent<RangeShadow>();
            auto &currentEntity = (rangeShadowP and rangeShadowP->entity == gameData.selected) ? rangeShadowP->entity
                                                                                               : entity;

            if (auto visibilityP = currentEntity->getComponent<Visibility>()) {
                auto &visibility = *visibilityP;
                auto positionP = currentEntity->getComponent<Position>();
                SDL_Rect *dstRect = visibility.getDstRect();
                SDL_Rect r = {int(dstRect->x * gameData.mapScale), int(dstRect->y * gameData.mapScale),
                              int(dstRect->w * gameData.mapScale), int(dstRect->h * gameData.mapScale)};
                if (positionP) {
                    auto &position = *positionP;
                    r.x = int((position.getX() + SIDEBAR_WIDTH) * gameData.mapScale - r.w / 2.0);
                    r.y = int(position.getY() * gameData.mapScale - r.h / 2.0);
                }

                if (gameData.selected == currentEntity and currentEntity != entity){
                    int currentEntityX, currentEntityY;
                    if (positionP) {
                        currentEntityX = r.x;
                        currentEntityY = r.y;
                    } else {
                        currentEntityX = r.x + int((dstRect->w / 2.0) * gameData.mapScale);
                        currentEntityY = r.y + int((dstRect->h / 2.0) * gameData.mapScale);
                    }
                    auto draggableP = currentEntity->getComponent<Draggable>();
                    bool isRed = draggableP ? !draggableP->isPlaceable : false;
                    float range = currentEntity->getComponent<Range>()->range;
                    filledCircleRGBA(gameData.renderer, currentEntityX, currentEntityY, range, isRed ? 255 : 0, 0, 0,
                                     100);
                    aacircleRGBA(gameData.renderer, currentEntityX, currentEntityY, range, isRed ? 255 : 0, 0, 0, 150);
                }
                if (entity == currentEntity)
                    SDL_RenderCopy(gameData.renderer, visibility.getTexture(), nullptr, &r);
            }
        }
    }
    SDL_RenderPresent(gameData.renderer);
}


