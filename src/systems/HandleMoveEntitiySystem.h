//
// Created by Ido Mozes on 10/07/2019.
//

#ifndef SDL_GAME_HANDLEMOVEENTITIYSYSTEM_H
#define SDL_GAME_HANDLEMOVEENTITIYSYSTEM_H
#include <algorithm>
#include "../System.h"
class HandleMoveEntitiySystem : public System {
public:
    void update(Entities *layers, GameData &gameData) override;
};
#endif //SDL_GAME_HANDLEMOVEENTITIYSYSTEM_H
