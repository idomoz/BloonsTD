//
// Created by Ido Mozes on 10/07/2019.
//

#include "RemoveEntitiesSystem.h"

void RemoveEntitiesSystem::update(Entities *layers, GameData &gameData) {
    for (int i = 0; i < N_LAYERS; ++i) {
        layers[i].erase(
                std::remove_if(layers[i].begin(), layers[i].end(), [layers](auto &entity) {
                    if(auto moveEntityEventP = entity->template getComponent<MoveEntityEvent>()) {
                        int newLayer = moveEntityEventP->toLayer;
                        entity->template removeComponent<MoveEntityEvent>();
                        layers[newLayer].emplace_back(std::move(entity));
                    }
                    return !entity or entity->template getComponent<RemoveEntityEvent>();
                }),
                layers[i].end());
    }
}