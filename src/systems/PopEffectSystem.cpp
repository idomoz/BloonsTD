//
// Created by Ido Mozes on 22/07/2019.
//

#include "PopEffectSystem.h"
const int range_from  = 0;
const int range_to    = 360;
std::random_device                  rand_dev;
std::mt19937                        generator(rand_dev());
std::uniform_int_distribution<int>  distr(range_from, range_to);

void PopEffectSystem::update(Entities *layers, GameData &gameData) {
    layers[POP_LAYER].clear();
    for (auto &bloon: layers[BLOONS_LAYER]) {
        if(bloon->getComponent<RemoveEntityEvent>() and bloon->getComponent<Seen>()){
            auto &position = bloon->getComponent<Position>()->value;
            if(position.X <0 or position.X >= MAP_WIDTH or position.Y <0 or position.Y >= MAP_HEIGHT)
                continue;
            auto pop = new Entity();
            pop->addComponent<Position>(position);
            SDL_Surface *surface = gameData.assets["Pop"];
            pop->addComponent<Visibility>(gameData.renderer,surface,SDL_Rect{0,0,int(surface->w/1.5)},distr(generator));
            layers[POP_LAYER].emplace_back(pop);
        }
        else
            bloon->addComponent<Seen>();
    }
}


