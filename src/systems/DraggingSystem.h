//
// Created by Ido Mozes on 09/07/2019.
//

#ifndef SDL_GAME_DRAGGINGSYSTEM_H
#define SDL_GAME_DRAGGINGSYSTEM_H
#include <iostream>
#include "../System.h"
class DraggingSystem : public System {
public:
    void update(std::vector<std::shared_ptr<Entity>> *layers, GameData &gameData) override;
};
#endif //SDL_GAME_DRAGGINGSYSTEM_H
