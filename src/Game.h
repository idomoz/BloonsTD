//
// Created by Ido Mozes on 18/06/2019.
//

#ifndef SDL2_GAME_GAME_H
#define SDL2_GAME_GAME_H

#include <iostream>
#include <memory>
#include <vector>

#include "SDL.h"
#include "SDL_image.h"

#include "Entity.h"
#include "System.h"
#include "systems/RenderSystem.h"
#include "components/Visibility.h"

class Game {
    bool isRunning;
    std::vector<std::unique_ptr<System>> systems;
    std::vector<std::unique_ptr<Entity>> entities;
    RenderSystem renderSystem;
public:
    explicit Game(bool fullscreen);

    ~Game();

    void handleEvents();

    void update();


    bool running() { return isRunning; }

};

#endif //SDL2_GAME_GAME_H
