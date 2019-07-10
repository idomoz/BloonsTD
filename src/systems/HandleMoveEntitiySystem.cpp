//
// Created by Ido Mozes on 10/07/2019.
//

#include "HandleMoveEntitiySystem.h"
void HandleMoveEntitiySystem::update(Entities *layers, GameData &gameData) {
    for (int i = 0; i < N_LAYERS; ++i) {
        for (auto &entity: layers[i]) {
                if(auto moveEntityEventP = entity->getComponent<MoveEntityEvent>()) {
                    int newLayer = moveEntityEventP->toLayer;
                    entity->removeComponent<MoveEntityEvent>();
                    layers[newLayer].emplace_back(std::move(entity));
                }
        }
    }
}