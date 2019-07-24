//
// Created by Ido Mozes on 10/07/2019.
//

#include "RemoveEntitiesSystem.h"

void RemoveEntitiesSystem::update(Entities *layers, GameData &gameData) {
    for (int i = 0; i < N_LAYERS; ++i) {
        layers[i].erase(
                std::remove_if(layers[i].begin(), layers[i].end(), [&layers, &gameData](auto &entity) {
                    if (auto moveEntityEventP = entity->template getComponent<MoveEntityEvent>()) {
                        int newLayer = moveEntityEventP->toLayer;
                        entity->template removeComponent<MoveEntityEvent>();
                        layers[newLayer].emplace_back(std::move(entity));
                    }
                    if (entity and entity->template getComponent<RangeShadow>() and
                        entity->template getComponent<RangeShadow>()->entity != gameData.selected)
                        return true;
                    return !entity or entity->template getComponent<RemoveEntityEvent>();
                }),
                layers[i].end());
    }
    if (layers[SEQUENCES_LAYER].empty() and layers[BLOONS_LAYER].empty()) {
        gameData.levelReady = gameData.levelRunning = false;
        if (gameData.level <= gameData.finalLevel and gameData.lives>0)
            gameData.level += 1;
        auto [texture,surface] = gameData.getTexture("Play");
        gameData.playButton->getComponent<Visibility>()->reloadTexture(texture,surface);
    }
}