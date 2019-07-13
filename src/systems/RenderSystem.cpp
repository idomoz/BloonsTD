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
                                       SDL_WINDOWPOS_CENTERED, /* NOLINT(hicpp-signed-bitwise)*/
                                       int((MAP_WIDTH + SIDEBAR_WIDTH + MENU_WIDTH) * gameData.mapScale),
                                       int(MAP_HEIGHT * gameData.mapScale),
                                       gameData.fullscreen ? SDL_WINDOW_FULLSCREEN : 0);
    gameData.renderer = SDL_CreateRenderer(gameData.window, -1, 0);
    SDL_SetRenderDrawColor(gameData.renderer, 255, 255, 255, 255);
    SDL_RenderSetScale(gameData.renderer, gameData.mapScale, gameData.mapScale);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");
}

void RenderSystem::update(Entities *layers, GameData &gameData) {

    SDL_RenderClear(gameData.renderer);
    for (int i = 0; i < N_LAYERS; ++i) {
        if (i == SEQUENCES_LAYER)
            continue;
        for (auto &entity: layers[i]) {
            auto rangeShadowP = entity->getComponent<RangeShadow>();
            auto &currentEntity = rangeShadowP ? rangeShadowP->entity : entity;
            if (auto visibilityP = currentEntity->getComponent<Visibility>()) {
                auto &visibility = *visibilityP;
                SDL_Rect *dstRect = visibility.getDstRect();
                SDL_Rect newDstRect = {dstRect->x, dstRect->y, dstRect->w, dstRect->h};
                SDL_Point entityCenter;

                auto positionP = currentEntity->getComponent<Position>();
                if (positionP) {
                    auto &position = *positionP;
                    entityCenter.x = position.value.X + SIDEBAR_WIDTH;
                    entityCenter.y = position.value.Y;
                    newDstRect.x = int(position.value.X + SIDEBAR_WIDTH - newDstRect.w / 2.0);
                    newDstRect.y = int(position.value.Y - newDstRect.h / 2.0);
                } else {
                    entityCenter.x = int(dstRect->x + dstRect->w / 2.0);
                    entityCenter.y = int(dstRect->y + dstRect->h / 2.0);
                }

                if (currentEntity != entity) {
                    auto draggableP = currentEntity->getComponent<Draggable>();
                    bool isRed = draggableP ? !draggableP->isPlaceable : false;
                    float range = currentEntity->getComponent<Range>()->value;
                    filledCircleRGBA(gameData.renderer, entityCenter.x, entityCenter.y, range, isRed ? 255 : 0, 0, 0,
                                     100);
                    aacircleRGBA(gameData.renderer, entityCenter.x, entityCenter.y, range, isRed ? 255 : 0, 0, 0, 150);
                }
                if (entity == currentEntity) {
                    SDL_RenderCopyEx(gameData.renderer, visibility.getTexture(), nullptr, &newDstRect, visibility.angle,
                                     nullptr, SDL_FLIP_NONE);

                }
            }
        }
    }
    SDL_RenderPresent(gameData.renderer);
}


