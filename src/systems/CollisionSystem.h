//
// Created by Ido Mozes on 11/07/2019.
//

#ifndef SDL_GAME_COLLISIONSYSTEM_H
#define SDL_GAME_COLLISIONSYSTEM_H
#include <tuple>
#include "../System.h"
class CollisionSystem : public System {
public:
    void update(Entities *layers, GameData &gameData) override;
};
#endif //SDL_GAME_COLLISIONSYSTEM_H
