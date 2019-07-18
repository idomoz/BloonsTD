//
// Created by Ido Mozes on 17/07/2019.
//

#ifndef SDL_GAME_CASHSYSTEM_H
#define SDL_GAME_CASHSYSTEM_H
#include "../System.h"
class CashSystem : public System {
public:
    void update(Entities *layers, GameData &gameData) override;
};
#endif //SDL_GAME_CASHSYSTEM_H
