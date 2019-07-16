//
// Created by Ido Mozes on 15/07/2019.
//

#ifndef SDL_GAME_DAMAGESYSTEM_H
#define SDL_GAME_DAMAGESYSTEM_H
#include "../System.h"
class DamageSystem : public System {
public:
    void update(Entities *layers, GameData &gameData) override;
};
#endif //SDL_GAME_DAMAGESYSTEM_H
