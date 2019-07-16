//
// Created by Ido Mozes on 09/07/2019.
//

#include "DraggingSystem.h"


void DraggingSystem::update(Entities *layers, GameData &gameData) {
    if (!gameData.isDragging)
        return;
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);
    mouseX = int(mouseX / gameData.mapScale);
    mouseY = int(mouseY / gameData.mapScale);
    for (int i = 0; i < N_LAYERS; ++i) {
        for (auto &entity: layers[i]) {
            if (auto components = entity->getComponents<Draggable, Visibility>()) {
                auto[draggable, visibility] = components.value();
                visibility.setPosition(mouseX - int(visibility.getDstRect()->w / 2.0),
                                       mouseY - int(visibility.getDstRect()->h / 2.0));
                bool freePosition = true;
                if (mouseX >= SIDEBAR_WIDTH + MAP_WIDTH or mouseX < SIDEBAR_WIDTH)
                    freePosition = true;
                else
                    for (int x = std::max(mouseX - SIDEBAR_WIDTH - 4, 0);
                         x < std::min(mouseX - SIDEBAR_WIDTH + 5, MAP_WIDTH - 1); ++x) {
                        for (int y = std::max(mouseY - 4, 0); y < std::min(mouseY + 5, MAP_HEIGHT - 1); ++y) {
                            if (gameData.mapData[x][y] == OBSTACLE or gameData.mapData[x][y] == TOWER) {
                                freePosition = false;
                                goto setIsPlaceable;
                            }
                        }
                    }
                setIsPlaceable:
                draggable.isPlaceable = freePosition;

            }
        }
    }
}