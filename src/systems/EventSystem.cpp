//
// Created by Ido Mozes on 08/07/2019.
//

#include "EventSystem.h"


void EventSystem::update(Entities *layers, GameData &gameData) {

    SDL_PumpEvents();
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT: {
                gameData.isRunning = false;
                break;
            }
            case SDL_MOUSEBUTTONDOWN: {

                int mouseX, mouseY, originalMouseX;
                SDL_GetMouseState(&mouseX, &mouseY);
                mouseX = originalMouseX = int(mouseX / gameData.mapScale);
                mouseY = int(mouseY / gameData.mapScale);
                bool entityClicked = false;
                Entities newEntities[N_LAYERS];
                for (int i = N_LAYERS - 1; i >= 0; --i) {
                    for (auto &entity: layers[i]) {
                        if (auto components = entity->getComponents<Action, Visibility>()) {
                            auto[action, visibility] = components.value();
                            if (action.disabled or (action.actionType != DROP and gameData.isDragging))
                                continue;
                            int entityX, entityY, w, h;
                            SDL_Rect *dstRect = visibility.getDstRect();
                            entityX = dstRect->x;
                            entityY = dstRect->y;
                            w = dstRect->w;
                            h = dstRect->h;

                            if (auto positionP = entity->getComponent<Position>()) {
                                auto &position = *positionP;
                                entityX = int(position.value.X - w / 2.0);
                                entityY = int(position.value.Y - h / 2.0);
                                mouseX = originalMouseX - SIDEBAR_WIDTH;
                            } else
                                mouseX = originalMouseX;

                            if (entityX <= mouseX and mouseX <= entityX + w and entityY <= mouseY and
                                mouseY <= entityY + h) {
                                switch (action.actionType) {
                                    case DRAG: {
                                        auto[type, kind, shotKind, range, attackInterval, pierce, damage, distance, cost] =
                                        entity->getComponents<Type, Kind, ShotKind, Range, AttackSpeed, Pierce, Damage, Distance, Cost>().value();
                                        auto draggable = new Entity();
                                        SDL_Surface *surface = gameData.assets[getSurfaceName(entity)];
                                        draggable->addComponent<Visibility>(gameData.renderer, surface, SDL_Rect{
                                                originalMouseX - int(surface->w / 3.0), mouseY - int(surface->h / 3.0),
                                                int(surface->w / 1.5), int(surface->h / 1.5)});
                                        draggable->addComponent<Draggable>();
                                        draggable->addComponent<Action>(DROP);
                                        draggable->addComponents(type, kind, shotKind, range, attackInterval,
                                                                 pierce, damage, distance, cost);
                                        draggable->addComponent<Strategy>(FIRST);
                                        if(auto spreadP = entity->getComponent<Spread>())
                                            draggable->addComponent<Spread>(*spreadP);
                                        if(auto gooP = entity->getComponent<Goo>())
                                            draggable->addComponent<Goo>(*gooP);
                                        if(entity->getComponent<Camo>())
                                            draggable->addComponent<Camo>();
                                        EntityP ptr(draggable);
                                        gameData.selected = ptr;
                                        auto rangeShadow = new Entity();
                                        rangeShadow->addComponent<RangeShadow>(ptr);
                                        newEntities[SHADOW_LAYER].emplace_back(rangeShadow);
                                        newEntities[MENU_LAYER].emplace_back(ptr);
                                        gameData.isDragging = true;
                                        goto entityClicked;
                                    }

                                    case CLICK: {
                                        switch (entity->getComponent<Kind>()->value) {
                                            case PLAY_FAST_FORWARD: {
                                                if (gameData.levelRunning)
                                                    gameData.FPS = 240 - gameData.FPS;
                                                else
                                                    gameData.levelRunning = true;
                                                visibility.loadTexture(gameData.renderer,
                                                                       gameData.assets[std::string("FastForward") +
                                                                                       (gameData.FPS == 180 ? "Enabled"
                                                                                                            : "")]);
                                            }
                                        }
                                        goto entityClicked;
                                    }
                                    case DROP: {
                                        auto &draggable = *entity->getComponent<Draggable>();
                                        if (draggable.isPlaceable) {
                                            entity->removeComponent<Draggable>();
                                            entity->getComponent<Action>()->actionType = SELECT;
                                            gameData.isDragging = false;
                                            if (mouseX > SIDEBAR_WIDTH + MAP_WIDTH or mouseX < SIDEBAR_WIDTH) {
                                                entity->addComponent<RemoveEntityEvent>();
                                                gameData.selected.reset();
                                            } else {
                                                for (int x = std::max(mouseX - SIDEBAR_WIDTH - 20, 0);
                                                     x < std::min(mouseX - SIDEBAR_WIDTH + 21, MAP_WIDTH); ++x) {
                                                    for (int y = std::max(mouseY - 20, 0);
                                                         y < std::min(mouseY + 21, MAP_HEIGHT); ++y) {
                                                        if (gameData.mapData[x][y] == FREE)
                                                            gameData.mapData[x][y] = TOWER;
                                                    }
                                                }
                                                if (i == MENU_LAYER) {
                                                    gameData.cash -= entity->getComponent<Cost>()->value;
                                                    entity->addComponent<MoveEntityEvent>(TOWERS_LAYER);
                                                    auto &visibility = *entity->getComponent<Visibility>();
                                                    SDL_Rect *dstRect = entity->getComponent<Visibility>()->getDstRect();
                                                    entity->addComponent<Position>(
                                                            dstRect->x - SIDEBAR_WIDTH + dstRect->w / 2,
                                                            dstRect->y + dstRect->h / 2);
                                                }
                                            }

                                        }
                                        goto entityClicked;
                                    }
                                    case SELECT: {
                                        if (entity == gameData.selected)
                                            goto entityClicked;
                                        gameData.selected = entity;
                                        auto rangeShadow = new Entity();
                                        rangeShadow->addComponent<RangeShadow>(entity);
                                        newEntities[SHADOW_LAYER].emplace_back(rangeShadow);
                                        goto entityClicked;
                                    }
                                }

                            }

                        }
                    }

                }
                gameData.selected.reset();
                entityClicked:
                for (int i = 0; i < N_LAYERS; ++i) {
                    if (!newEntities[i].empty())
                        layers[i] += newEntities[i];
                }
                break;
            }
            default:
                break;
        }
    }

}
