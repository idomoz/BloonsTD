//
// Created by Ido Mozes on 07/07/2019.
//

#include "BloonsSpawnSystem.h"


void BloonsSpawnSystem::update(Entities *layers, GameData &gameData) {
    if (!gameData.levelRunning)
        return;
    for (auto &entity: layers[SEQUENCES_LAYER]) {
        auto[sequence, kind] = entity->getComponents<Sequence, Kind>().value();
        auto[regrowP, camoP, fortifiedP] = entity->getComponentsP<Regrow, Camo, Fortified>();
        int amount = sequence.getAmountReady();
        if (amount == SEQUENCE_FINISHED) {
            entity->addComponent<RemoveEntityEvent>();
            continue;
        }
        for (int j = 0; j < amount; ++j) {
            EntityP bloon(new Entity());
            bloon->addComponent<Type>(BLOON_T);
            bloon->addComponent<Position>(gameData.startingPoint);
            bloon->addComponent<PathIndex>(0);

            bloon->addComponents(kind);
            if (regrowP)
                bloon->addComponent<Regrow>(*regrowP);
            if (camoP)
                bloon->addComponent<Camo>();
            if (fortifiedP)
                bloon->addComponent<Fortified>();
            bloon->addComponent<Lives>(getBloonProperty<TOTAL_LIVES>(bloon));
            auto[texture, surface] = gameData.getTexture(getSurfaceName(bloon));
            bloon->addComponent<Visibility>(texture, surface,
                                            SDL_Rect{0, 0, int(surface->w / 3), int(surface->h / 3)});
            bloon->addComponent<Range>(kind.value < MOAB ? std::max(surface->w / 6, surface->h / 6) :
                                       (surface->w / 6.0 + surface->h / 6.0) / 2);
            layers[BLOONS_LAYER].emplace_back(bloon);
        }
    }

}
