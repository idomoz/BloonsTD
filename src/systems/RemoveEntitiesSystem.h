//
// Created by Ido Mozes on 10/07/2019.
//

#ifndef SDL_GAME_REMOVEENTITIESSYSTEM_H
#define SDL_GAME_REMOVEENTITIESSYSTEM_H
#include <algorithm>
#include "../System.h"
class RemoveEntitiesSystem : public System {
public:
    void update(Entities *layers, GameData &gameData) override;
};
#endif //SDL_GAME_REMOVEENTITIESSYSTEM_H
