//
// Created by Ido Mozes on 08/07/2019.
//

#include "EventSystem.h"


void EventSystem::update(std::vector<std::shared_ptr<Entity>> *layers, GameData &gameData) {

    SDL_PumpEvents();
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT: {
                gameData.isRunning = false;
                break;
            }
            case SDL_MOUSEBUTTONDOWN: {
                std::vector<std::shared_ptr<Entity>> newEntities;
                std::vector<std::shared_ptr<Entity>> entitiesToRemove;
                int mouseX, mouseY, originalMouseX;
                SDL_GetMouseState(&mouseX, &mouseY);
                mouseX = originalMouseX = int(mouseX / gameData.mapScale);
                mouseY = int(mouseY / gameData.mapScale);
                for (int i = N_LAYERS - 1; i >= 0; --i) {
                    for (auto &entity: layers[i]) {
                        if (auto components = entity->getComponents<Action,Visibility>()) {
                            auto [action,visibility] = components.value();
                            if (action.disabled or (action.type == DRAG and gameData.isDragging))
                                continue;
                            int entityX, entityY, w, h;
                            entityX = visibility.getDstRect()->x;
                            entityY = visibility.getDstRect()->y;
                            w = visibility.getDstRect()->w;
                            h = visibility.getDstRect()->h;

                            if (auto positionP=entity->getComponent<Position>()) {
                                auto &position = *positionP;
                                entityX = int(position.getX() - w / 2.0);
                                entityY = int(position.getY() - h / 2.0);
                                mouseX = originalMouseX - SIDEBAR_WIDTH;
                            } else
                                mouseX = originalMouseX;

                            if (entityX <= mouseX and mouseX <= entityX + w and entityY <= mouseY and
                                mouseY <= entityY + h) {
                                switch (action.type) {
                                    case DRAG: {
                                        auto &kind = *entity->getComponent<Kind>();
                                        auto draggable = new Entity();
                                        SDL_Surface *surface = gameData.assets[kind.kind];
                                        draggable->addComponent<Visibility>(gameData.renderer, surface, SDL_Rect{
                                                originalMouseX - int(surface->w / 2.0), mouseY - int(surface->h / 2.0),
                                                surface->w, surface->h}, 100);
                                        draggable->addComponent<Draggable>();
                                        draggable->addComponent<Action>(DROP);
                                        draggable->addComponent<Kind>(kind);
                                        newEntities.emplace_back(draggable);
                                        gameData.isDragging = true;
                                        goto entityClicked;
                                    }

                                    case CLICK: {
                                        goto entityClicked;
                                    }
                                    case DROP: {
                                        auto &draggable = *entity->getComponent<Draggable>();
                                        if (draggable.isPlaceable) {
                                            entity->removeComponent<Draggable>();
                                            gameData.isDragging = false;
                                        }
                                        break;
                                    }
                                }

                            }
                        }
                    }
                }
                entityClicked:
                layers[3].insert(layers[3].end(), std::make_move_iterator(newEntities.begin()),
                                 std::make_move_iterator(newEntities.end()));
                break;
            }
            default:
                break;
        }
    }

}
