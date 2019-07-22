//
// Created by Ido Mozes on 17/07/2019.
//

#include "MenuSystem.h"

void MenuSystem::update(Entities *layers, GameData &gameData) {
    for (auto &entity: layers[MENU_LAYER]) {
        auto[kindP, actionP] = entity->getComponentsP<Kind, Action>();
        if (kindP and (!actionP or (actionP->actionType != DRAG and actionP->actionType != DROP)))
            switch (kindP->value) {
                case UPGRADES_BACKGROUND:
                case NEXT_STRATEGY:
                case PREVIOUS_STRATEGY:
                case UPGRADE_PATH_1:
                case UPGRADE_PATH_2:
                case UPGRADE_PATH_3:
                    entity->getComponent<Visibility>()->hidden = bool(!gameData.selected or gameData.isDragging);
                    break;
            }
        if (kindP and actionP and actionP->actionType == CLICK and gameData.selected and !gameData.isDragging) {
            switch (kindP->value) {
                case UPGRADE_PATH_1:
                case UPGRADE_PATH_2:
                case UPGRADE_PATH_3: {
                    auto &upgrades = *gameData.selected->getComponent<Upgrades>();
                    int path = kindP->value - UPGRADE_PATH_1;
                    auto &visibility = *entity->getComponent<Visibility>();
                    auto upgradeP_P = entity->getComponent<UpgradeP>();
                    if (!upgrades.paths[path].empty()) {
                        auto &currentUpgrade = upgrades.paths[path][0];
                        if (!upgradeP_P or upgradeP_P->value != &currentUpgrade or
                            upgradeP_P->name != currentUpgrade.name or (!actionP->disabled and currentUpgrade.locked)) {
                            entity->addComponent<UpgradeP>(&currentUpgrade);
                            if (currentUpgrade.locked)
                                currentUpgrade.surface = gameData.assets[currentUpgrade.surfaceName + "Locked"];
                            SDL_Surface *surface = currentUpgrade.surface;
                            visibility.setDstRect(
                                    {75 - surface->w / 34, 143 + 135 * path - surface->h / 17, surface->w / 17, 0});
                            visibility.loadTexture(gameData.renderer, surface);
                        }
                        actionP->disabled = currentUpgrade.locked;
                        visibility.hidden = false;
                    } else
                        visibility.hidden = true;
                    break;
                }
            }
        }
        if (auto components = entity->getComponents<Cost, Action, Visibility>()) {
            auto[cost, action, visibility] = components.value();
            switch (action.actionType) {
                case DRAG:
                    bool disabled = gameData.cash < cost.value;
                    if (disabled != action.disabled) {
                        action.disabled = disabled;
                        visibility.loadTexture(gameData.renderer,
                                               gameData.assets[getSurfaceName(entity) + (disabled ? "Disabled" : "")]);
                    }

            }

        }
    }
}
