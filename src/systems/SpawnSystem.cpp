//
// Created by Ido Mozes on 07/07/2019.
//

#include "SpawnSystem.h"


void SpawnSystem::update(Entities *layers, GameData &gameData) {
    for (int i = 0; i < N_LAYERS; ++i) {
        Entities newEntities;
        for (auto &entity: layers[i]) {
            if (auto components = entity->getComponents<Sequence, Kind, Speed>()) {
                auto[sequence, kind, speed] = components.value();
                int amount = sequence.getAmountReady();
                for (int j = 0; j < amount; ++j) {
                    auto *newEntity = new Entity();
                    newEntity->addComponent<Kind>(kind);
                    newEntity->addComponent<Position>(gameData.startingPoint.X, gameData.startingPoint.Y);
                    newEntity->addComponent<PathIndex>(0);
                    newEntity->addComponent<Speed>(speed.speed);
                    SDL_Surface *surface = gameData.assets[kind.kind];
                    newEntity->addComponent<Visibility>(gameData.renderer, surface,
                                                        SDL_Rect{gameData.startingPoint.X, gameData.startingPoint.Y,
                                                                 surface->w / 3, surface->h / 3});
                    newEntities.emplace_back(newEntity);
                }
            }
        }
        layers[i] += newEntities;
    }
}
