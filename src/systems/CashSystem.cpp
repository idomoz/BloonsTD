//
// Created by Ido Mozes on 17/07/2019.
//

#include "CashSystem.h"

void CashSystem::update(Entities *layers, GameData &gameData) {
    for (auto &entity: layers[MENU_LAYER]) {
        if (auto components = entity->getComponents<Cost, Action, Visibility>()) {
            auto[cost, action, visibility] = components.value();
            if (action.actionType == DRAG or action.actionType == CLICK) {
                bool disabled = gameData.cash < cost.value;
                if(disabled != action.disabled){
                    action.disabled = disabled;
                    visibility.loadTexture(gameData.renderer,gameData.assets[getSurfaceName(entity) + (disabled? "Disabled" : "")]);
                }

            }
        }
    }
}
