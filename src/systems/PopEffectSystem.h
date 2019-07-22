//
// Created by Ido Mozes on 22/07/2019.
//

#ifndef SDL_GAME_POPEFFECTSYSTEM_H
#define SDL_GAME_POPEFFECTSYSTEM_H
#include <random>
#include "../System.h"
class PopEffectSystem : public System {
public:
    void update(Entities *layers, GameData &gameData) override;
};
#endif //SDL_GAME_POPEFFECTSYSTEM_H
