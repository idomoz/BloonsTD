//
// Created by Ido Mozes on 07/07/2019.
//

#include "SpawnSystem.h"

uint64_t SpawnSystem::mask = createMask({ComponentType::SEQUENCE, ComponentType::KIND});

void SpawnSystem::update(std::vector<std::shared_ptr<Entity>> *layers, GameData &gameData) {
    for (int i = 0; i < N_LAYERS; ++i) {
        std::vector<std::shared_ptr<Entity>> newEntities;
        for (auto &entity: layers[i]) {
            if (entity->hasComponents(mask)) {
                int amount = entity->getComponent<Sequence>().getAmountReady();
                for (int i = 0; i < amount; ++i) {
                    auto *newEntity = new Entity();
                    newEntity->addComponent<Kind>(entity->getComponent<Kind>().kind);
                    newEntity->addComponent<Position>(gameData.startingPoint.X, gameData.startingPoint.Y);
                    newEntity->addComponent<PathIndex>(0);
                    newEntity->addComponent<Speed>(entity->getComponent<Speed>().speed);
                    SDL_Surface *surface = gameData.assets[entity->getComponent<Kind>().kind];
                    newEntity->addComponent<Visibility>(gameData.renderer, surface,
                                                        SDL_Rect{gameData.startingPoint.X, gameData.startingPoint.Y,
                                                                 surface->w / 3, surface->h / 3});
                    newEntities.emplace_back(newEntity);
                }
            }
        }
        layers[i].insert(layers[i].end(), std::make_move_iterator(newEntities.begin()),
                         std::make_move_iterator(newEntities.end()));
    }
}
