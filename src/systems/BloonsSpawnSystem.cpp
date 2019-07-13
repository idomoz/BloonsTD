//
// Created by Ido Mozes on 07/07/2019.
//

#include "BloonsSpawnSystem.h"


void BloonsSpawnSystem::update(Entities *layers, GameData &gameData) {
    for (auto &entity: layers[SEQUENCES_LAYER]) {
        auto[sequence, kind, speed] = entity->getComponents<Sequence, Kind, Speed>().value();
        int amount = sequence.getAmountReady();
        for (int j = 0; j < amount; ++j) {
            auto *bloon = new Entity();
            bloon->addComponent<Type>(BLOON_T);
            bloon->addComponent<Kind>(kind);
            bloon->addComponent<Position>(gameData.startingPoint.X, gameData.startingPoint.Y);
            bloon->addComponent<PathIndex>(0);
            bloon->addComponent<Speed>(speed.value);

            SDL_Surface *surface = gameData.assets[kind.value];
            bloon->addComponent<Visibility>(gameData.renderer, surface,
                                            SDL_Rect{int(gameData.startingPoint.X),
                                                     int(gameData.startingPoint.Y),
                                                     int(surface->w / 3), int(surface->h / 3)});
            bloon->addComponent<Range>(std::max(surface->w / 6, surface->h / 6));
            layers[BLOONS_LAYER].emplace_back(bloon);
        }
    }

}
