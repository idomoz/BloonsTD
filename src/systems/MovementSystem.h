//
// Created by Ido Mozes on 02/07/2019.
//

#ifndef SDL_GAME_MOVEMENTSYSTEM_H
#define SDL_GAME_MOVEMENTSYSTEM_H
#include <iostream>
#include "../components/Visibility.h"
#include "../components/Velocity.h"
#include "../components/Position.h"
#include "../components/PathIndex.h"
#include "../components/Speed.h"
#include "../GameData.h"
#include "../System.h"
class MovementSystem : public System{
public:
    MovementSystem()= default;

    ~MovementSystem()= default;

    void update(std::vector<std::shared_ptr<Entity>> &entities,  GameData & gameData) override;
};
#endif //SDL_GAME_MOVEMENTSYSTEM_H
