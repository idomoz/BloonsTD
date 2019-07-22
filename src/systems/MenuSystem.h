//
// Created by Ido Mozes on 17/07/2019.
//

#ifndef SDL_GAME_MENUSYSTEM_H
#define SDL_GAME_MENUSYSTEM_H
#include "../System.h"
class MenuSystem : public System {
public:
    void update(Entities *layers, GameData &gameData) override;
};
#endif //SDL_GAME_MENUSYSTEM_H
