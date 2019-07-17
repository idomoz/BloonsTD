//
// Created by Ido Mozes on 07/07/2019.
//

#include "BloonsSpawnSystem.h"


void BloonsSpawnSystem::update(Entities *layers, GameData &gameData) {
    for (auto &entity: layers[SEQUENCES_LAYER]) {
        auto[sequence, kind] = entity->getComponents<Sequence, Kind>().value();
        auto [regrowP,camoP,fortifiedP] = entity->getComponentsP<Regrow,Camo,Fortified>();
        int amount = sequence.getAmountReady();
        for (int j = 0; j < amount; ++j) {
            EntityP bloon(new Entity());
            bloon->addComponent<Type>(BLOON_T);
            bloon->addComponent<Position>(gameData.startingPoint);
            bloon->addComponent<PathIndex>(0);

            bloon->addComponents(kind);
            if (regrowP)
                bloon->addComponent<Regrow>();
            if (camoP)
                bloon->addComponent<Camo>();
            if (fortifiedP)
                bloon->addComponent<Fortified>();
            bloon->addComponent<Lives>(getBloonProperty<TOTAL_LIVES>(bloon));
            SDL_Surface *surface = gameData.assets[getSurfaceName(bloon)];
            bloon->addComponent<Visibility>(gameData.renderer, surface,
                                            SDL_Rect{0, 0, int(surface->w / 3), int(surface->h / 3)});
            bloon->addComponent<Range>(std::max(surface->w / 6, surface->h / 6));
            layers[BLOONS_LAYER].emplace_back(bloon);
        }
    }

}
