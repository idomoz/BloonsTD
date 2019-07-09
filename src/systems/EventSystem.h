//
// Created by Ido Mozes on 08/07/2019.
//

#ifndef SDL_GAME_EVENTSYSTEM_H
#define SDL_GAME_EVENTSYSTEM_H

#include <iostream>
#include "../System.h"
#include "../systems/RenderSystem.h"



class EventSystem : public System {
public:
    void update(std::vector<std::shared_ptr<Entity>> *layers, GameData &gameData) override;
};

#endif //SDL_GAME_EVENTSYSTEM_H
